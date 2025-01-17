 // Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TowerController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Character/PlayerTank.h"

void ATowerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Find distance to the tank
	if (InFireRange())
	{
		RotateTurret(Tank->GetActorLocation());	
	}
}

void ATowerController::BeginPlay()
{
	Super::BeginPlay();

	Tank = Cast<APlayerTank>(UGameplayStatics::GetPlayerPawn(this, 0));
	
	GetWorldTimerManager().SetTimer(
		FireRateTimerHandle, 
		[this]()
		{
			if (UAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				FGameplayTagContainer DontCare;
				ASC->TryActivateAbilitiesByTag(FGameplayTagContainer(FGameplayTag::RequestGameplayTag("Ability.Enemy.Shoot")), true);
			}
		}, 
		FireRate, 
		true
	);
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

