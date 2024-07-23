// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerController.h"
#include "TankController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ATowerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Find distance to the tank
	if (InFireRange())
	{
		RotateTurret(Tank->GetActorLocation());	
	}
}

void  ATowerController::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();

}

void ATowerController::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<ATankController>(UGameplayStatics::GetPlayerPawn(this,0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATowerController::CheckFireCondition, FireRate, true);
	
}

void ATowerController::CheckFireCondition()
{
	if (Tank == nullptr)
	{
		return;
	}
	if (InFireRange() && Tank->bAlive)
	{
		Fire();
	}
}

bool ATowerController::InFireRange()
{
	// Checks tank is not Null
	if (Tank)
	{
		Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		// Check if tank is in range
		if (Distance <= FireRange)
		{
			return true;
		}
	}
	return false;
}

