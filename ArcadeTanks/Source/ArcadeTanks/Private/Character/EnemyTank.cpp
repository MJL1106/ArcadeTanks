// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyTank.h"

AEnemyTank::AEnemyTank()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MovementSpeed = 125.0f;
	TurnRate = 20.0f; 
}

void AEnemyTank::BeginPlay()
{
	Super::BeginPlay();
}
