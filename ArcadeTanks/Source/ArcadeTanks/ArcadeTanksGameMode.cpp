// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "TankController.h"
#include "TowerController.h"
#include "ArcadeTanksPlayerController.h"

void AArcadeTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ArcadeTanksPlayerController)
		{
			ArcadeTanksPlayerController->SetPlayerEnabledState(false);
		}
	}
	else if (ATowerController* DestroyedTower = Cast<ATowerController>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
	}
}

void AArcadeTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AArcadeTanksGameMode::HandleGameStart()
{
	Tank = Cast<ATankController>(UGameplayStatics::GetPlayerPawn(this, 0));
	ArcadeTanksPlayerController = Cast<AArcadeTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if (ArcadeTanksPlayerController)
	{
		ArcadeTanksPlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerEnableTimerHandle;
		FTimerDelegate PlayerEnableTimerDelegate = FTimerDelegate::CreateUObject(
			ArcadeTanksPlayerController,
			&AArcadeTanksPlayerController::SetPlayerEnabledState,
			true
		);
		GetWorldTimerManager().SetTimer(
			PlayerEnableTimerHandle, 
			PlayerEnableTimerDelegate,
			StartDelay,
			false
		);
	}
	
}