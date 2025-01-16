// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TowerBasePawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/TankAbilitySystemComponent.h"
#include "AbilitySystem/TankAttributeSet.h"
#include "Game/ArcadeTanksGameMode.h"
#include "Particles/ParticleSystem.h"

class AArcadeTanksGameMode;
// Sets default values
ATowerBasePawn::ATowerBasePawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(BaseMesh);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	AbilitySystemComponent = CreateDefaultSubobject<UTankAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UTankAttributeSet>(TEXT("AttributeSet"));
}

void ATowerBasePawn::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		GiveDefaultAbilities();
	}
}

void ATowerBasePawn::InitializeAttributes()
{
	if (AbilitySystemComponent && InitialAttributes)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitialAttributes, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATowerBasePawn::GiveDefaultAbilities()
{
	UTankAbilitySystemComponent* TurretASC = CastChecked<UTankAbilitySystemComponent>(AbilitySystemComponent);
	
	if (HasAuthority() && TurretASC)
	{
		TurretASC->AddCharacterAbilities(InitialAbilities);
	}
}


void ATowerBasePawn::HandleDestruction()
{
	if (DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	}
	if (DeathCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DeathCameraShakeClass);
	}
	
	AArcadeTanksGameMode* ArcadeTanksGameMode = Cast<AArcadeTanksGameMode>(UGameplayStatics::GetGameMode(this));
	ArcadeTanksGameMode->ActorDied(this);
	
	Destroy();
}

float ATowerBasePawn::GetHealth_Implementation() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float ATowerBasePawn::GetMaxHealth_Implementation() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

void ATowerBasePawn::RotateTurret(FVector LookAtTarget)
{
	FVector ToTarget = LookAtTarget - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);

	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(
			TurretMesh->GetComponentRotation(), 
			LookAtRotation, 
			UGameplayStatics::GetWorldDeltaSeconds(this), 
			5.f));
}