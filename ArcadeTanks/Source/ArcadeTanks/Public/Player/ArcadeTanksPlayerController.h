// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcadeTanksPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API AArcadeTanksPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AArcadeTanksPlayerController();
	
	virtual void Tick(float DeltaTime) override;

	void SetPlayerEnabledState(bool bPlayerEnabled);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	// Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputAction* RotateAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

private:
	UPROPERTY()
	class ATankCharacterBase* ControlledTank;

	// Input Handler Functions
	void HandleMoveInput(const struct FInputActionValue& Value);
	void HandleRotateInput(const struct FInputActionValue& Value);
	void HandleTurretRotation();

	// Helper Functions
	FVector GetMouseWorldLocation() const;

};
