// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TankBasePawn.h"
#include "TankController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API ATankController : public ATankBasePawn
{
	GENERATED_BODY()
	
public:

	ATankController();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	void Move(float Value);
};
