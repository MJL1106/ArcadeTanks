// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TankAttributeSet.h"

#include "Net/UnrealNetwork.h"

UTankAttributeSet::UTankAttributeSet()
{
	// Set default values
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitArmor(50.0f);
}

void UTankAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTankAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTankAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTankAttributeSet, Armor, COND_None, REPNOTIFY_Always);
}

void UTankAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTankAttributeSet, Health, OldHealth);
}

void UTankAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTankAttributeSet, MaxHealth, OldMaxHealth);
}

void UTankAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTankAttributeSet, Armor, OldArmor);
}