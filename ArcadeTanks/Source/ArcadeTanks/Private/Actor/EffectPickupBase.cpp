// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/EffectPickupBase.h"

#include "Character/TankCharacterBase.h"
#include "Components/SphereComponent.h"
#include "GameplayEffect.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/TankAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffect.h"

AEffectPickupBase::AEffectPickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	CollisionComponent->SetSphereRadius(100.0f);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->bAutoActivate = true;

	EffectDuration = 10.0f;
	bShouldRotate = true;
	RotationRate = 180.0f;
	bShouldFloat = true;
	FloatHeight = 50.0f;
	FloatSpeed = 2.0f;

	FadeOutDuration = 1.0f;
	bIsBeingDestroyed = false;
	DestroyTimer = 0.0f;
}

void AEffectPickupBase::BeginPlay()
{
	Super::BeginPlay();

	InitialLocation = GetActorLocation();
	RunningTime = 0.0f;

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEffectPickupBase::OnOverlapBegin);

	if (PickupParticleSystem)
	{
		ParticleSystemComponent->SetTemplate(PickupParticleSystem);
	}

	OriginalScale = GetActorScale3D();
}

void AEffectPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UpdatePickupMovement(DeltaTime);
	if (bIsBeingDestroyed)
	{
		UpdateDestroyEffect(DeltaTime);
	}
}

void AEffectPickupBase::UpdatePickupMovement(float DeltaTime)
{
	if (!bShouldRotate && !bShouldFloat)
	{
		return;
	}

	RunningTime += DeltaTime;

	FVector NewLocation = InitialLocation;
	FRotator NewRotation = GetActorRotation();

	if (bShouldRotate)
	{
		NewRotation.Yaw = FMath::Fmod(NewRotation.Yaw + (RotationRate * DeltaTime), 360.0f);
	}

	if (bShouldFloat)
	{
		NewLocation.Z = InitialLocation.Z + (FMath::Sin(RunningTime * FloatSpeed) * FloatHeight);
	}

	SetActorLocationAndRotation(NewLocation, NewRotation);
}

void AEffectPickupBase::UpdateDestroyEffect(float DeltaTime)
{
	DestroyTimer += DeltaTime;
	float Alpha = FMath::Clamp(DestroyTimer / FadeOutDuration, 0.0f, 1.0f);

	float InverseAlpha = 1.0f - Alpha;

	FVector NewScale = OriginalScale * InverseAlpha;
	SetActorScale3D(NewScale);
	
	if (ParticleSystemComponent)
	{
		ParticleSystemComponent->SetColorParameter(FName("Color"), 
			FLinearColor(1.0f, 1.0f, 1.0f, InverseAlpha));
	}
	if (Alpha >= 1.0f)
	{
		Destroy();
	}
}

void AEffectPickupBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

	if (!OtherActor || bIsBeingDestroyed)
	{
		return;
	}

	ATankCharacterBase* Tank = Cast<ATankCharacterBase>(OtherActor);
	if (!Tank)
	{
		return;
	}

	UAbilitySystemComponent* ASC = Tank->GetAbilitySystemComponent();
	if (!ASC || !EffectClass)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(EffectClass, 1.0f, EffectContext);
    
	if (!SpecHandle.IsValid())
	{
		return;
	}

	if (EffectDuration > 0.0f)
	{
		SpecHandle.Data.Get()->SetDuration(EffectDuration, true);
	}

	FActiveGameplayEffectHandle ActiveHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	if (ActiveHandle.WasSuccessfullyApplied())
	{
		if (PickupVFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				PickupVFX,
				GetActorLocation(),
				GetActorRotation()
			);
		}

		if (PickupSFX)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickupSFX,
				GetActorLocation()
			);
		}
		bIsBeingDestroyed = true;
		DestroyTimer = 0.0f;

		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

