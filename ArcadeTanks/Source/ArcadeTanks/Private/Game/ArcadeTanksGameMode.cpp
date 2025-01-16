// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ArcadeTanksGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Character/TowerController.h"
#include "Character/PlayerTank.h"
#include "Player/ArcadeTanksPlayerController.h"


void AArcadeTanksGameMode::ActorDied(AActor* DeadActor)
{
	if (DeadActor == Tank)
	{
		if (ArcadeTanksPlayerController)
		{
			ArcadeTanksPlayerController->SetPlayerEnabledState(false);
		}
		GameOver(false);
	}
	else
	{
		TargetTowers--;
		if (TargetTowers == 0)
		{
			ArcadeTanksPlayerController->SetPlayerEnabledState(false);
			GameOver(true);
		}
	}
}

void AArcadeTanksGameMode::BeginPlay()
{
	Super::BeginPlay();

	HandleGameStart();
}

void AArcadeTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	Tank = Cast<APlayerTank>(UGameplayStatics::GetPlayerPawn(this, 0));
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

int32 AArcadeTanksGameMode::GetTargetTowerCount()
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATowerController::StaticClass(), Towers);

	return Towers.Num();
}