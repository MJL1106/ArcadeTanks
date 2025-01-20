// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseShootAbility.h"
#include "EnemyShootAbility.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class ARCADETANKS_API UEnemyShootAbility : public UBaseShootAbility
{
	GENERATED_BODY()


protected:
	virtual bool PerformAbilityChecks() const override;

private:
	bool IsTargetInRange() const;
	AActor* GetCurrentTarget() const;
	
};
