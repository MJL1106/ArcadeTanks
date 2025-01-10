// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TankCharacterBase.h"

#include "AbilitySystem/TankAbilitySystemComponent.h"
#include "AbilitySystem/TankAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ATankCharacterBase::ATankCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	TankBaseMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TankBaseMesh"));
	TankBaseMesh->SetupAttachment(GetRootComponent());

	TurretMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(TankBaseMesh);

	
	AbilitySystemComponent = CreateDefaultSubobject<UTankAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UTankAttributeSet>(TEXT("AttributeSet"));

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
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
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (TSubclassOf<UGameplayAbility>& DefaultAbility : DefaultAbilities)
		{
			AbilitySystemComponent->GiveAbility(
				FGameplayAbilitySpec(DefaultAbility, 1, INDEX_NONE, this));
		}
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
}

void ATankCharacterBase::RotateTurret(FVector TargetLocation)
{
	const FRotator NewRotation = GetTurretRotation(TargetLocation);
	TurretMesh->SetWorldRotation(FMath::RInterpTo(
		TurretMesh->GetComponentRotation(),
		NewRotation,
		GetWorld()->GetDeltaSeconds(),
		5.0f
	));
}

FRotator ATankCharacterBase::GetTurretRotation(const FVector& TargetLocation) const
{
	FVector ToTarget = TargetLocation - TurretMesh->GetComponentLocation();
	FRotator LookAtRotation = FRotator(0.0f, ToTarget.Rotation().Yaw - 90.0f, 0.0f);
	return LookAtRotation;
}

void ATankCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

