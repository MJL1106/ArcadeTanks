// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TankGameplayAbility.h"
#include "TankDamageGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API UTankDamageGameplayAbility : public UTankGameplayAbility
{
	GENERATED_BODY()
	
public:
	UTankDamageGameplayAbility();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	float BaseDamage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UFUNCTION(BlueprintCallable, Category = "Damage")
	virtual void ApplyDamageToTarget(AActor* TargetActor);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTagContainer RequiredTargetTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTagContainer BlockedTargetTags;
};
