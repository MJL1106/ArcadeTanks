// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TankCharacterBase.h"
#include "PlayerTank.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API APlayerTank : public ATankCharacterBase
{
	GENERATED_BODY()

public:
	APlayerTank();

	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* Camera;

protected:
	virtual void BeginPlay() override;

	// Camera Properties
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float SpringArmLength = 600.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FRotator CameraRotation = FRotator(-30.0f, 0.0f, 0.0f);
};
