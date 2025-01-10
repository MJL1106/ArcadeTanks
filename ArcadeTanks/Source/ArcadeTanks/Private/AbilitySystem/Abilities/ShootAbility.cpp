// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ShootAbility.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


#include "Character/TankCharacterBase.h"

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

    // Play the shooting montage if we have one
    if (ShootMontage)
    {
        UAbilityTask_PlayMontageAndWait* MontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
            this,
            NAME_None,
            ShootMontage,
            1.0f,    // Play Rate
            NAME_None, // Start Section Name
            false    // StopWhenAbilityEnds
        );

        MontageTask->OnCompleted.AddDynamic(this, &UShootAbility::OnMontageCompleted);
        MontageTask->OnInterrupted.AddDynamic(this, &UShootAbility::OnMontageCompleted);
        MontageTask->OnCancelled.AddDynamic(this, &UShootAbility::OnMontageCompleted);
        MontageTask->ReadyForActivation();
        
        if (ProjectileSpawnDelay > 0.0f)
        {
            FTimerHandle SpawnTimerHandle;
            FTimerDelegate SpawnDelegate;
            SpawnDelegate.BindUObject(this, &UShootAbility::SpawnProjectile);
            GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, SpawnDelegate, ProjectileSpawnDelay, false);
        }
        else
        {
            SpawnProjectile();
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
    
    FVector SpawnLocation = Tank->GetTurretMesh()->GetSocketLocation(FName("ProjectileSocket"));
    
    FRotator SpawnRotation = Tank->GetTurretMesh()->GetComponentRotation();
    SpawnRotation.Yaw += 90.f;

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Tank;
    SpawnParams.Instigator = Tank;

    AActor* Projectile = GetWorld()->SpawnActor<AActor>(
        ProjectileClass,
        SpawnLocation,
        SpawnRotation,
        SpawnParams
    );

    if (Projectile)
    {
        if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Projectile->GetRootComponent()))
        {
            PrimComp->AddImpulse(SpawnRotation.Vector() * ProjectileSpeed);
        }
    }
}

void UShootAbility::OnMontageCompleted()
{
    if (HasAuthority(&CurrentActivationInfo))
    {
        EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
    }
}