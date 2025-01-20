// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseShootAbility.h"
#include "ShootAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API UShootAbility : public UBaseShootAbility
{
	GENERATED_BODY()

public:
	UShootAbility();

protected:
	virtual bool PerformAbilityChecks() const override;
};
