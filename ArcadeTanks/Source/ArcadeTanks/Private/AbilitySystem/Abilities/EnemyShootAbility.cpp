// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EnemyShootAbility.h"

#include "Projectile.h"
#include "TowerController.h"
#include "Character/EnemyTank.h"
#include "Character/TankCharacterBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

void UEnemyShootAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    if (IsTargetInRange())
    {
        AActor* OwningActor = GetAvatarActorFromActorInfo();
        if (ATankCharacterBase* TankCharacter = Cast<ATankCharacterBase>(OwningActor))
        {
            if (ShootMontage)
            {
                USkeletalMeshComponent* BaseMesh = TankCharacter->GetBaseMesh();
                if (!BaseMesh)
                {
                    EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
                    return;
                }

                BaseMesh->GetAnimInstance()->Montage_Play(ShootMontage, 1.0f);
            }
        }
        if (ProjectileSpawnDelay > 0.0f)
        {
            FTimerHandle SpawnTimerHandle;
            FTimerDelegate SpawnDelegate;
            SpawnDelegate.BindUObject(this, &UEnemyShootAbility::SpawnProjectile);
            GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, SpawnDelegate, ProjectileSpawnDelay, false);
        }
        else
        {
            SpawnProjectile();
        }
    }
    
    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UEnemyShootAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UEnemyShootAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
    float CooldownDuration = 0.0f;
    AActor* OwningActor = GetAvatarActorFromActorInfo();
    
    if (const ATankCharacterBase* Character = Cast<ATankCharacterBase>(OwningActor))
    {
        CooldownDuration = Character->FireRate;
    }
    else if (const ATowerController* Tower = Cast<ATowerController>(OwningActor))
    {
        CooldownDuration = Tower->FireRate;
    }

    if (CooldownDuration > 0.0f)
    {
        UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
        if (CooldownEffect)
        {
            const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(GetClass(), GetAbilityLevel());
            if (SpecHandle.IsValid())
            {
                SpecHandle.Data.Get()->SetDuration(CooldownDuration, true);
                FActiveGameplayEffectHandle ActiveHandle = ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
            }
        }
    }
}

void UEnemyShootAbility::SpawnProjectile()
{
    AActor* OwningActor = GetAvatarActorFromActorInfo();
    FVector SocketLocation;
    FRotator SocketRotation;
    if (!OwningActor || !ProjectileClass)
    {
        return;
    }

    const USkeletalMeshComponent* Mesh = Cast<USkeletalMeshComponent>(OwningActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
    const ATowerController* TowerEnemy = Cast<ATowerController>(OwningActor);
    const UStaticMeshComponent* Tower = TowerEnemy->GetTurretMesh();
    
    if (!Mesh && !Tower)
    {
        return;
    }

    if (Mesh)
    {
        SocketLocation = Mesh->GetSocketLocation(ProjectileSocketName);
        SocketRotation = Mesh->GetSocketRotation(ProjectileSocketName);
    }
    else if (Tower)
    {
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
        }
        else if (ATowerController* TowerBase = Cast<ATowerController>(OwningActor))
        {
            Projectile->ProjectileMovementComponent->MaxSpeed = TowerBase->MaxBulletSpeed;
            Projectile->ProjectileMovementComponent->InitialSpeed = TowerBase->InitialBulletSpeed;
        }
        UGameplayStatics::FinishSpawningActor(Projectile, FTransform(SocketRotation, SpawnLocation));
    }
}

bool UEnemyShootAbility::IsTargetInRange() const
{
    const AActor* Target = GetCurrentTarget();
    if (!Target)
    {
        return false;
    }

   const float Distance = FVector::Dist(
        GetAvatarActorFromActorInfo()->GetActorLocation(),
        Target->GetActorLocation()
    );

    float TargetingRange = 0.0f;
    
    if (const ATowerController* Tower = Cast<ATowerController>(GetAvatarActorFromActorInfo()))
    {
        TargetingRange = Tower->FireRange;
    }
    else if (const AEnemyTank* EnemyTank = Cast<AEnemyTank>(GetAvatarActorFromActorInfo()))
    {
        TargetingRange = EnemyTank->FireRange;
    }
    
    return Distance <= TargetingRange;
}

AActor* UEnemyShootAbility::GetCurrentTarget() const
{
    return UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}


