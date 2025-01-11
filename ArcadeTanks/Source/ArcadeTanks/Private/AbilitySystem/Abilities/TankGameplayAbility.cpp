// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TankGameplayAbility.h"

#include "Character/TankCharacterBase.h"

UTankGameplayAbility::UTankGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

ATankCharacterBase* UTankGameplayAbility::GetTankCharacter() const
{
	return Cast<ATankCharacterBase>(GetAvatarActorFromActorInfo());
}