// Fill out your copyright notice in the Description page of Project Settings.


#include "TankGameplayTags.h"
#include "GameplayTagsManager.h"

FTankGameplayTags FTankGameplayTags::GameplayTags;

void FTankGameplayTags::InitializeNativeGameplayTags()
{
	// Access the central Gameplay Tags Manager
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();


	/* Abilities */
	
	GameplayTags.Ability_Tank_Shoot = Manager.AddNativeGameplayTag(
		FName("Ability.Tank.Shoot"),
FString("Shoot ability for tanks")
	);

	/* End Abilities*/

	/* Actor Types */
	
	GameplayTags.Actor_Type_Enemy = Manager.AddNativeGameplayTag(
		FName("Actor.Type.Enemy"),
FString("Enemy Actor")
	);
	
	/* End Actor Types */
}