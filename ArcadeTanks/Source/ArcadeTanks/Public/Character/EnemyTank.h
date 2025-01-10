// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TankCharacterBase.h"
#include "EnemyTank.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API AEnemyTank : public ATankCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyTank();

protected:
	virtual void BeginPlay() override;

	// Enemy specific properties
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float DetectionRange = 1000.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float FireRange = 800.0f;
};
