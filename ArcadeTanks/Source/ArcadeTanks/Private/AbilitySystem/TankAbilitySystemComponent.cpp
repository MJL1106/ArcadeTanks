// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TankAbilitySystemComponent.h"

#include "TankGameplayTags.h"
#include "AbilitySystem/Abilities/TankGameplayAbility.h"

UTankAbilitySystemComponent::UTankAbilitySystemComponent()
{
	SetIsReplicated(true);
	UAbilitySystemComponent::SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

void UTankAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTankAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	
}

void UTankAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UTankGameplayAbility* TankAbility = Cast<UTankGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(TankAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}
