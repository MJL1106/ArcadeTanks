// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TankDamageGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemBlueprintLibrary.h"

UTankDamageGameplayAbility::UTankDamageGameplayAbility()
{
	BaseDamage = 10.0f;
}

void UTankDamageGameplayAbility::ApplyDamageToTarget(AActor* TargetActor)
{
	if (!TargetActor || !DamageEffectClass)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return;
	}
	
	if (!RequiredTargetTags.IsEmpty())
	{
		if (!TargetASC->HasAllMatchingGameplayTags(RequiredTargetTags))
		{
			return;
		}
	}

	if (!BlockedTargetTags.IsEmpty())
	{
		if (TargetASC->HasAnyMatchingGameplayTags(BlockedTargetTags))
		{
			return;
		}
	}
	
	FGameplayEffectContextHandle ContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());
	FGameplayEffectSpec* DamageSpec = new FGameplayEffectSpec(DamageEffectClass.GetDefaultObject(), ContextHandle, 1.0f);
    
	if (DamageSpec)
	{
		DamageSpec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), BaseDamage);
		
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpec);
	}
}