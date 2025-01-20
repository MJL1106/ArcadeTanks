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

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	CollisionComponent->SetSphereRadius(100.0f);
	CollisionComponent->SetMobility(EComponentMobility::Movable);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->Mobility = EComponentMobility::Movable;
	
	EffectDuration = 10.0f;
	bShouldRotate = true;
	RotationRate = 180.0f;
	bShouldFloat = true;
	FloatHeight = 50.0f;
	FloatSpeed = 2.0f;
}

void AEffectPickupBase::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AEffectPickupBase::OnOverlapBegin);
	InitialLocation = GetActorLocation();
	RunningTime = 0.0f;
}

void AEffectPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UpdatePickupMovement(DeltaTime);
}

void AEffectPickupBase::UpdatePickupMovement(float DeltaTime)
{
	RunningTime += DeltaTime;

	if (bShouldRotate)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += RotationRate * DeltaTime;
		SetActorRotation(NewRotation);
	}

	if (bShouldFloat)
	{
		FVector NewLocation = InitialLocation;
		NewLocation.Z += FMath::Sin(RunningTime * FloatSpeed) * FloatHeight;
		SetActorLocation(NewLocation);
	}
}

void AEffectPickupBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{

    if (!OtherActor)
    {
        return;
    }

    ATankCharacterBase* Tank = Cast<ATankCharacterBase>(OtherActor);
    if (!Tank)
    {
        return;
    }

    UAbilitySystemComponent* ASC = Tank->GetAbilitySystemComponent();
    if (!ASC)
    {
        return;
    }

    if (!EffectClass)
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

    	Destroy();
    }
}

