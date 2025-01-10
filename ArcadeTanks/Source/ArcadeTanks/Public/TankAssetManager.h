// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TankAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API UTankAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:

	static UTankAssetManager& Get();

protected:

	virtual void StartInitialLoading() override;
};
