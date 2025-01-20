// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseShootAbility.generated.h"

class AProjectile;
/**
 * 
 */
UCLASS()
class ARCADETANKS_API UBaseShootAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UBaseShootAbility();

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
	class ATankCharacterBase* GetTankCharacter() const;

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

	virtual bool PerformAbilityChecks() const { return true; }
	void ExecuteShootSequence();

private:
	
	UFUNCTION()
	void SpawnProjectile() const;
	
	float GetFireRate(AActor* OwningActor) const;
};
