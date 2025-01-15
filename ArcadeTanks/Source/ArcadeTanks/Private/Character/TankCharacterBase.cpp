// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TankCharacterBase.h"

#include "AbilitySystem/TankAbilitySystemComponent.h"
#include "AbilitySystem/TankAttributeSet.h"
#include "Game/ArcadeTanksGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ATankCharacterBase::ATankCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	TankBaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TankBaseMesh"));
	TankBaseMesh->SetupAttachment(GetRootComponent());

	AbilitySystemComponent = CreateDefaultSubobject<UTankAbilitySystemComponent>(TEXT("ASC"));
	AttributeSet = CreateDefaultSubobject<UTankAttributeSet>(TEXT("AttributeSet"));

	bUseControllerRotationYaw = false;

	if (Movement)
	{
		Movement->bOrientRotationToMovement = false;
		Movement->MaxWalkSpeed = MovementSpeed;
	}
}

void ATankCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		GiveDefaultAbilities();
	}
}

void ATankCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
	}
}

void ATankCharacterBase::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);
		if (SpecHandle.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ATankCharacterBase::GiveDefaultAbilities()
{
	UTankAbilitySystemComponent* TankASC = CastChecked<UTankAbilitySystemComponent>(AbilitySystemComponent);
	
	if (HasAuthority() && TankASC)
	{
		TankASC->AddCharacterAbilities(DefaultAbilities);
	}
}

void ATankCharacterBase::HandleDestruction()
{
	if (DestructionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			DestructionEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	
	if (DestructionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DestructionSound, GetActorLocation());
	}
	
	if (DestructionCameraShake)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DestructionCameraShake);
	}

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	AArcadeTanksGameMode* ArcadeTanksGameMode = Cast<AArcadeTanksGameMode>(UGameplayStatics::GetGameMode(this));
	ArcadeTanksGameMode->ActorDied(this);
}

float ATankCharacterBase::GetHealth_Implementation() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetHealth();
	}
	return 0.0f;
}

float ATankCharacterBase::GetMaxHealth_Implementation() const
{
	if (AttributeSet)
	{
		return AttributeSet->GetMaxHealth();
	}
	return 0.0f;
}

void ATankCharacterBase::RotateTurret(FVector TargetLocation)
{
	FRotator TargetRotation = GetTurretRotation(TargetLocation);
    
	TurretTargetRotation = FMath::RInterpTo(
	   TurretTargetRotation,  
	   TargetRotation,        
	   GetWorld()->GetDeltaSeconds(),
	   5.0f                   
	);
	
	TurretTargetRotation.Normalize();
}

FRotator ATankCharacterBase::GetTurretRotation(const FVector& TargetLocation) const
{
	FVector SocketLocation = TankBaseMesh->GetSocketLocation(FName("ProjectileSocket"));
	FVector ToTarget = TargetLocation - SocketLocation;
	
	FRotator WorldRotation = ToTarget.Rotation();
	
	FRotator LocalRotation = FRotator(
		0.0f,
		(WorldRotation - TankBaseMesh->GetComponentRotation()).Yaw - 90.0f,
		0.0f
	);
    
	LocalRotation.Normalize();
	return LocalRotation;
}

void ATankCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

