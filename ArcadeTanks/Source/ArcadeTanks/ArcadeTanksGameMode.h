// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArcadeTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API AArcadeTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()
	

public:
	void ActorDied(AActor* DeadActor);

protected:
	virtual void BeginPlay() override;

private:
	
	class ATankController* Tank;

	class AArcadeTanksPlayerController* ArcadeTanksPlayerController;
};
