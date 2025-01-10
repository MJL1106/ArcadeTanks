// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "TankAbilitySystemComponent.generated.h"

UCLASS()
class GAME_API UTankAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UTankAbilitySystemComponent();

	// Initialize the ASC actor info
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;

	// Custom function for tank-specific ability handling
	UFUNCTION(BlueprintCallable, Category = "Tank|Abilities")
	void InitializeComponent();

protected:
	virtual void BeginPlay() override;
};
