// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "TankGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API UTankGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UTankGameplayAbility();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;

protected:
	class ATankCharacterBase* GetTankCharacter() const;
};
