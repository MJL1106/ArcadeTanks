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

	/*TurretMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(TankBaseMesh);*/

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
    
	// Ensure the rotation stays within -180 to 180 degrees
	TurretTargetRotation.Normalize();
}

FRotator ATankCharacterBase::GetTurretRotation(const FVector& TargetLocation) const
{
	FVector SocketLocation = TankBaseMesh->GetSocketLocation(FName("TurretSocket"));
	FVector ToTarget = TargetLocation - SocketLocation;
    
	// Get the world rotation
	FRotator WorldRotation = ToTarget.Rotation();
    
	// Convert to local space but ONLY use the Yaw component
	FRotator LocalRotation = FRotator(
		0.0f,  // Keep pitch at 0
		(WorldRotation - TankBaseMesh->GetComponentRotation()).Yaw - 90.0f,  // Only use Yaw
		0.0f   // Keep roll at 0
	);
    
	LocalRotation.Normalize();
	return LocalRotation;
}

void ATankCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

