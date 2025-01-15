// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ShootAbility.h"

#include "Projectile.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


#include "Character/TankCharacterBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UShootAbility::UShootAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UShootAbility::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    ATankCharacterBase* Tank = Cast<ATankCharacterBase>(ActorInfo->AvatarActor);
    if (!Tank || !ProjectileClass)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
        return;
    }
    
    if (BaseShootMontage)
    {
        ATankCharacterBase* Character = Cast<ATankCharacterBase>(GetAvatarActorFromActorInfo());
        if (!Character)
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid Character for montage!"));
            return;
        }

        USkeletalMeshComponent* BaseMesh = Character->GetBaseMesh();
        if (!BaseMesh)
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid Mesh for montage!"));
            return;
        }

        BaseMesh->GetAnimInstance()->Montage_Play(BaseShootMontage, 1.0f);
        
        if (ProjectileSpawnDelay > 0.0f)
        {
            FTimerHandle SpawnTimerHandle;
            FTimerDelegate SpawnDelegate;
            SpawnDelegate.BindUObject(this, &UShootAbility::SpawnProjectile);
            GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, SpawnDelegate, ProjectileSpawnDelay, false);
            EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        }
        else
        {
            SpawnProjectile();
            EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        }
    }
    else
    {
        SpawnProjectile();
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
    }
}

void UShootAbility::SpawnProjectile()
{
    ATankCharacterBase* Tank = Cast<ATankCharacterBase>(GetAvatarActorFromActorInfo());
    if (!Tank || !ProjectileClass)
    {
        return;
    }
    
    FVector SocketLocation = Tank->GetBaseMesh()->GetSocketLocation(FName("ProjectileSocket"));
    FRotator SocketRotation = Tank->GetBaseMesh()->GetSocketRotation(FName("ProjectileSocket"));
    
    FVector ForwardVector = SocketRotation.Vector();
    FVector SpawnLocation = SocketLocation + (ForwardVector * 10.0f);
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Tank;
    SpawnParams.Instigator = Tank;
    
    AProjectile* Projectile = GetWorld()->SpawnActorDeferred<AProjectile>(
        ProjectileClass,                   
        FTransform(SocketRotation, SpawnLocation),
        Tank,                              
        Tank,                   
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
    );

    if (Projectile)
    {
        Projectile->ProjectileMovementComponent->MaxSpeed = Tank->MaxBulletSpeed;
        Projectile->ProjectileMovementComponent->InitialSpeed = Tank->InitialBulletSpeed;
        
        UGameplayStatics::FinishSpawningActor(Projectile, FTransform(SocketRotation, SpawnLocation));
    }
}