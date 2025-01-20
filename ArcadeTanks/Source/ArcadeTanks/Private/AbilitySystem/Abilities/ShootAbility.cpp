// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ShootAbility.h"

#include "Abilities/Tasks/AbilityTask.h"
#include "Character/TankCharacterBase.h"


UShootAbility::UShootAbility()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UShootAbility::PerformAbilityChecks() const
{
    return Cast<ATankCharacterBase>(GetAvatarActorFromActorInfo()) != nullptr;
}