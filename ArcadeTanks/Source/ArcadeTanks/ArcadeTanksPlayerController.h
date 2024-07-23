// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArcadeTanksPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARCADETANKS_API AArcadeTanksPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void SetPlayerEnabledState(bool bPlayerEnabled);
	
};
