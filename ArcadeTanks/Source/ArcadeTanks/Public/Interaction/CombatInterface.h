// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ARCADETANKS_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual void HandleDestruction() = 0;
	virtual void UpdateMovementSpeed(const float SpeedMultiplier) {}

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	float GetHealth() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Combat")
	float GetMaxHealth() const;
};
