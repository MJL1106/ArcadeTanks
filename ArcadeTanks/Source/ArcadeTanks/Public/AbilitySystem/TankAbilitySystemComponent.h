// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TankAbilitySystemComponent.generated.h"

UCLASS()
class ARCADETANKS_API UTankAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UTankAbilitySystemComponent();

	// Initialize the ASC actor info
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

protected:
	virtual void BeginPlay() override;
};
