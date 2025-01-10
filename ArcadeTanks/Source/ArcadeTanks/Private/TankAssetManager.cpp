// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAssetManager.h"

#include "TankGameplayTags.h"
#include "AbilitySystemGlobals.h"

UTankAssetManager& UTankAssetManager::Get()
{
	check(GEngine);
	
	UTankAssetManager* AuraAssetManager = Cast<UTankAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UTankAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FTankGameplayTags::InitializeNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}