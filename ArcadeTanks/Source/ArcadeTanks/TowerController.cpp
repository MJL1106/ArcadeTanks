// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerController.h"
#include "TankController.h"
#include "Kismet/GameplayStatics.h"

void ATowerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Find distance to the tank
	if (Tank)
	{
		Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		// Check to see if Tank is in range
		if (Distance <= FireRange)
		{
			RotateTurret(Tank->GetActorLocation());
		}
	}
}

void ATowerController::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATankController>(UGameplayStatics::GetPlayerPawn(this,0));
	
}


