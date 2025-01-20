// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/EnemyShootAbility.h"

#include "Character/TowerController.h"
#include "Character/EnemyTank.h"
#include "Character/TankCharacterBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

bool UEnemyShootAbility::PerformAbilityChecks() const
{
	return IsTargetInRange();
}

bool UEnemyShootAbility::IsTargetInRange() const
{
	const AActor* Target = GetCurrentTarget();
	if (!Target)
	{
		return false;
	}

	const float Distance = FVector::Dist(
		GetAvatarActorFromActorInfo()->GetActorLocation(),
		Target->GetActorLocation()
	);

	float TargetingRange = 0.0f;
    
	if (const ATowerController* Tower = Cast<ATowerController>(GetAvatarActorFromActorInfo()))
	{
		TargetingRange = Tower->FireRange;
	}
	else if (const AEnemyTank* EnemyTank = Cast<AEnemyTank>(GetAvatarActorFromActorInfo()))
	{
		TargetingRange = EnemyTank->FireRange;
	}
    
	return Distance <= TargetingRange;
}

AActor* UEnemyShootAbility::GetCurrentTarget() const
{
	return UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

