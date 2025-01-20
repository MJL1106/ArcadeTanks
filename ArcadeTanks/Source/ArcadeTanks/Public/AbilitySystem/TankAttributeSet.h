// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "TankAttributeSet.generated.h"

// Use macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class ATankCharacterBase;

UCLASS()
class ARCADETANKS_API UTankAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTankAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// Health
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UTankAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UTankAttributeSet, MaxHealth)

	// Armor
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UTankAttributeSet, Armor)

	// Fire Rate Multiplier
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_FireRateMultiplier)
	FGameplayAttributeData FireRateMultiplier;
	ATTRIBUTE_ACCESSORS(UTankAttributeSet, FireRateMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MovementSpeedMultiplier)
	FGameplayAttributeData MovementSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UTankAttributeSet, MovementSpeedMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_DamageMultiplier)
	FGameplayAttributeData DamageMultiplier;
	ATTRIBUTE_ACCESSORS(UTankAttributeSet, DamageMultiplier)
	
protected:
	// Rep notifies
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Armor(const FGameplayAttributeData& OldArmor);

	UFUNCTION()
	virtual void OnRep_FireRateMultiplier(const FGameplayAttributeData& OldFireRateMultiplier);

	UFUNCTION()
	virtual void OnRep_MovementSpeedMultiplier(const FGameplayAttributeData& OldMovementSpeedMultiplier);

	UFUNCTION()
	virtual void OnRep_DamageMultiplier(const FGameplayAttributeData& OldDamageMultiplier);
};