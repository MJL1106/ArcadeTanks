// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/BaseShootAbility.h"

#include "AbilitySystem/TankAttributeSet.h"
#include "Actor/Projectile.h"
#include "Character/TankCharacterBase.h"
#include "Character/TowerController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UBaseShootAbility::UBaseShootAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UBaseShootAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	if (!PerformAbilityChecks())
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	ExecuteShootSequence();
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UBaseShootAbility::ExecuteShootSequence()
{
	AActor* OwningActor = GetAvatarActorFromActorInfo();
	ATankCharacterBase* TankCharacter = Cast<ATankCharacterBase>(OwningActor);
    
	if (TankCharacter && ShootMontage)
	{
		USkeletalMeshComponent* BaseMesh = TankCharacter->GetBaseMesh();
		if (BaseMesh)
		{
			BaseMesh->GetAnimInstance()->Montage_Play(ShootMontage, 1.0f);
		}
	}

	if (ProjectileSpawnDelay > 0.0f)
	{
		FTimerHandle SpawnTimerHandle;
		FTimerDelegate SpawnDelegate;
		SpawnDelegate.BindUObject(this, &UBaseShootAbility::SpawnProjectile);
		GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, SpawnDelegate, ProjectileSpawnDelay, false);
	}
	else
	{
		SpawnProjectile();
	}
}

void UBaseShootAbility::SpawnProjectile()
{
    AActor* OwningActor = GetAvatarActorFromActorInfo();
    if (!OwningActor || !ProjectileClass)
    {
        return;
    }

    FVector SocketLocation;
    FRotator SocketRotation;

    if (const ATankCharacterBase* TankCharacter = Cast<ATankCharacterBase>(OwningActor))
    {
        USkeletalMeshComponent* BaseMesh = TankCharacter->GetBaseMesh();
        if (!BaseMesh) return;
        
        SocketLocation = BaseMesh->GetSocketLocation(ProjectileSocketName);
        SocketRotation = BaseMesh->GetSocketRotation(ProjectileSocketName);
    }
    else if (const ATowerController* TowerEnemy = Cast<ATowerController>(OwningActor))
    {
        const UStaticMeshComponent* Tower = TowerEnemy->GetTurretMesh();
        if (!Tower) return;
        
        SocketLocation = Tower->GetSocketLocation(ProjectileSocketName);
        SocketRotation = Tower->GetSocketRotation(ProjectileSocketName);
    }

    const FVector ForwardVector = SocketRotation.Vector();
    const FVector SpawnLocation = SocketLocation + (ForwardVector * 10.0f);
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = OwningActor;
    SpawnParams.Instigator = Cast<APawn>(OwningActor);
    
    AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
        ProjectileClass,
        FTransform(SocketRotation, SpawnLocation),
        OwningActor,
        Cast<APawn>(OwningActor),
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
    );

    if (Projectile)
    {
        if (const ATankCharacterBase* TankBase = Cast<ATankCharacterBase>(OwningActor))
        {
            Projectile->ProjectileMovementComponent->MaxSpeed = TankBase->MaxBulletSpeed;
            Projectile->ProjectileMovementComponent->InitialSpeed = TankBase->InitialBulletSpeed;
        	
        	const float Damage = Projectile->Damage;
        	Projectile->Damage = Damage * TankBase->GetAttributeSet()->GetDamageMultiplier();
        }
        else if (ATowerController* TowerBase = Cast<ATowerController>(OwningActor))
        {
            Projectile->ProjectileMovementComponent->MaxSpeed = TowerBase->MaxBulletSpeed;
            Projectile->ProjectileMovementComponent->InitialSpeed = TowerBase->InitialBulletSpeed;
        }
        
        UGameplayStatics::FinishSpawningActor(Projectile, FTransform(SocketRotation, SpawnLocation));
    }
}

float UBaseShootAbility::GetFireRate(AActor* OwningActor) const
{
	if (const ATankCharacterBase* Character = Cast<ATankCharacterBase>(OwningActor))
	{
		float BaseCooldownDuration = Character->FireRate;
		if (UTankAttributeSet* AttributeSet = Character->GetAttributeSet())
		{
			const float FireRateMultiplier = AttributeSet->GetFireRateMultiplier();
			const float NewDuration = BaseCooldownDuration / FireRateMultiplier;
			BaseCooldownDuration = NewDuration;
		}
		return BaseCooldownDuration;
	}
	if (const ATowerController* Tower = Cast<ATowerController>(OwningActor))
	{
		return Tower->FireRate;
	}
	return 0.0f;
}

void UBaseShootAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}


void UBaseShootAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);

	const float CooldownDuration = GetFireRate(GetAvatarActorFromActorInfo());

	if (CooldownDuration > 0.0f)
	{
		UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
		if (CooldownEffect)
		{
			const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownEffect->GetClass(), GetAbilityLevel());
			if (SpecHandle.IsValid())
			{
				SpecHandle.Data.Get()->SetDuration(CooldownDuration, true);
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
			}
		}
	}
}

ATankCharacterBase* UBaseShootAbility::GetTankCharacter() const
{
	return Cast<ATankCharacterBase>(GetAvatarActorFromActorInfo());
}
