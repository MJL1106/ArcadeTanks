// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TankGameplayAbility.h"
#include "ShootAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API UShootAbility : public UTankGameplayAbility
{
	GENERATED_BODY()

public:
	UShootAbility();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	TSubclassOf<class AActor> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* BaseShootMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	float ProjectileSpawnDelay = 0.0f;

private:
	UFUNCTION()
	void SpawnProjectile();
};
