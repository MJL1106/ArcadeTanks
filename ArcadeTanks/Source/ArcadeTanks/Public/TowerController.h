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

	void HandleDestruction();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	class ATankController* Tank;

	float Distance;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRange = 300.f;

	FTimerHandle FireRateTimerHandle;
	float FireRate = 2.f;
	void CheckFireCondition();

	bool InFireRange();
};
