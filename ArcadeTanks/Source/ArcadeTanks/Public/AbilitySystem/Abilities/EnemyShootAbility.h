// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TankGameplayAbility.h"
#include "EnemyShootAbility.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class ARCADETANKS_API UEnemyShootAbility : public UTankGameplayAbility
{
	GENERATED_BODY()

	virtual void ActivateAbility(
	   const FGameplayAbilitySpecHandle Handle,
	   const FGameplayAbilityActorInfo* ActorInfo,
	   const FGameplayAbilityActivationInfo ActivationInfo,
	   const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting")
	UAnimMontage* ShootMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting")
	float ProjectileSpawnDelay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shooting")
	FName ProjectileSocketName = FName("ProjectileSocket");

	virtual void ApplyCooldown(
		const FGameplayAbilitySpecHandle Handle, 
		const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo) const;

private:
	void SpawnProjectile();
	bool IsTargetInRange() const;
	AActor* GetCurrentTarget() const;
};
