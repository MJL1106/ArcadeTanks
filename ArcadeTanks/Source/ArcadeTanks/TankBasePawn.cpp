// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBasePawn.h"

// Sets default values
ATankBasePawn::ATankBasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATankBasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATankBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

