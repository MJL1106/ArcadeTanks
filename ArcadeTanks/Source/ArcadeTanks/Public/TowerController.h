// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankBasePawn.h"
#include "TowerController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API ATowerController : public ATankBasePawn
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 300.f;

	FTimerHandle FireRateTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate = 2.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float TurnRate = 45.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float InitialBulletSpeed = 1300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float MaxBulletSpeed = 1300.f;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:

	class APlayerTank* Tank;

	float Distance;
	
	bool InFireRange();
};
