// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TankAbilitySystemComponent.h"

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

void UTankAbilitySystemComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
}