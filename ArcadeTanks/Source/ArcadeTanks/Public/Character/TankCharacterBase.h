// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "TankCharacterBase.generated.h"

UCLASS()
class ARCADETANKS_API ATankCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ATankCharacterBase();


	// Ability System
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return AbilitySystemComponent; }
	class UTankAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank")
	USkeletalMeshComponent* TankBaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank")
	USkeletalMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	// Movement Properties
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float TurnRate = 45.0f;

	// Effects
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* DestructionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* DestructionSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> DestructionCameraShake;

	// Functions
	UFUNCTION(BlueprintCallable, Category = "Tank")
	virtual void HandleDestruction();

	UFUNCTION(BlueprintCallable, Category = "Tank")
	virtual void RotateTurret(FVector TargetLocation);

	// Getter functions for movement properties
	float GetMovementSpeed() const { return MovementSpeed; }
	float GetTurnRate() const { return TurnRate; }

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY()
	class UTankAttributeSet* AttributeSet;

	// Default abilities for tanks
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<class UGameplayAbility>> DefaultAbilities;

	// Effect to apply on spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;

private:
	FRotator GetTurretRotation(const FVector& TargetLocation) const;
	void InitializeAttributes();
	void GiveDefaultAbilities();

};
