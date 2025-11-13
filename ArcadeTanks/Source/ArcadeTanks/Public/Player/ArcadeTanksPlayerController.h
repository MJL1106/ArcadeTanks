// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
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

	UFUNCTION(BlueprintCallable, Category = "Input")
	void HandleShootInput();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual bool InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex) override;

	// Input Actions
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* RotateAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchMoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchAimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	UInputAction* TouchShootAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UUserWidget> MobileControlsWidgetClass;

	UPROPERTY()
	UUserWidget* MobileControlsWidget;

private:
	UPROPERTY()
	class ATankCharacterBase* ControlledTank;

	// Input Handler Functions
	void HandleMoveInput(const struct FInputActionValue& Value);
	void HandleRotateInput(const struct FInputActionValue& Value);
	void HandleTurretRotation();

	void HandleMoveForwardAxis(float Value);
	void HandleTurnAxis(float Value);
	FVector2D RightJoystickInput;
	void HandleRotateTurretAxis(float Value);
	void HandleRotateTurretYAxis(float Value);

	void MoveForward(float Value);
	void MoveRight(float Value);
	//void LookUp(float Value);
	//void LookRight(float Value);

	// Helper Functions
	FVector GetMouseWorldLocation() const;

	// Touch input
	void HandleTouchMoveTriggered(const FInputActionValue& Value);
	void HandleTouchAimTriggered(const FInputActionValue& Value);
	void HandleTouchShootTriggered(const FInputActionValue& Value);

	FVector2D TouchMoveStartLocation;
	FVector TouchAimWorldLocation;
	bool bIsTouchAiming = false;

};
