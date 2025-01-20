// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TankAttributeSet.h"

#include "Character/TowerBasePawn.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

class ATankBasePawn;

UTankAttributeSet::UTankAttributeSet()
{
	// Set default values
	InitHealth(60.0f);
	InitMaxHealth(60.0f);
	InitArmor(50.0f);
	InitFireRateMultiplier(1.0f);
}

void UTankAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTankAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTankAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTankAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UTankAttributeSet, FireRateMultiplier, COND_None, REPNOTIFY_Always);
}

void UTankAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float NewHealth = FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth());
		SetHealth(NewHealth);

		if (NewHealth <= 0.0f)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwningActor()))
			{
				CombatInterface->HandleDestruction();
			}
		}
	}
	else if (Data.EvaluatedData.Attribute == GetFireRateMultiplierAttribute())
	{
		const float OldFireRate = GetFireRateMultiplier();
		const float NewFireRateMultiplier = FMath::Max(GetFireRateMultiplier(), 0.1f);
		SetFireRateMultiplier(NewFireRateMultiplier);
	}
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

void UTankAttributeSet::OnRep_FireRateMultiplier(const FGameplayAttributeData& OldFireRateMultiplier)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UTankAttributeSet, FireRateMultiplier, OldFireRateMultiplier);
}