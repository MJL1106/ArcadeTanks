// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * FTankGameplayTags
 *
 * Singleton containing native Gameplay Tags for Tank Abilities
 */
struct FTankGameplayTags
{
public:
	static const FTankGameplayTags& Get() { return GameplayTags; }
	
	static void InitializeNativeGameplayTags();

	/* Abilities */
	FGameplayTag Ability_Tank_Shoot;
	/* End Abilities */

	/* Actor Type */
	FGameplayTag Actor_Type_Enemy;
	/* End Actor Type */
private:
	// Internal singleton instance
	static FTankGameplayTags GameplayTags;
};