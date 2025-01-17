// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/TankAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "TankCharacterBase.generated.h"

UCLASS()
class ARCADETANKS_API ATankCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	UAbilitySystemComponent* AbilitySystemComponent;

	// Combat Interface
	virtual void HandleDestruction() override;
	virtual float GetHealth_Implementation() const override;
	virtual float GetMaxHealth_Implementation() const override;

	// Movement Properties
	UPROPERTY()
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MovementSpeed = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float TurnRate = 45.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Tank")
	FRotator TurretTargetRotation;
	
	UFUNCTION(BlueprintCallable, Category = "Tank")
	virtual void RotateTurret(FVector TargetLocation);

	// Shoot properties
	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float FireRate = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float InitialBulletSpeed = 1300.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float MaxBulletSpeed = 1300.f;
	
	// Effects
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem* DestructionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	USoundBase* DestructionSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> DestructionCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	// Getter functions for movement properties
	float GetMovementSpeed() const { return MovementSpeed; }
	float GetTurnRate() const { return TurnRate; }
	USkeletalMeshComponent* GetBaseMesh() const { return TankBaseMesh; }
	FRotator GetTurretRotation(const FVector& TargetLocation) const;
	
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

	FName TurretBoneName = "BoneTurret";

private:
	void InitializeAttributes();
	void GiveDefaultAbilities();

};
