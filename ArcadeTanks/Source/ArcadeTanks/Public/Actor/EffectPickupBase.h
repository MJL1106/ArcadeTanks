// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "EffectPickupBase.generated.h"

class UGameplayEffect;
class USphereComponent;

UCLASS()
class ARCADETANKS_API AEffectPickupBase : public AActor
{
	GENERATED_BODY()
	
public:    
	AEffectPickupBase();

	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// The gameplay effect to apply
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	TSubclassOf<UGameplayEffect> EffectClass;

	// Duration of the effect (0 for instant effects like health pickup)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Pickup")
	float EffectDuration;

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	// VFX/SFX
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UNiagaraSystem* PickupVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	USoundBase* PickupSFX;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleSystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	float FadeOutDuration;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* PickupParticleSystem;

	// Movement properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bShouldRotate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (EditCondition = "bShouldRotate"))
	float RotationRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	bool bShouldFloat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (EditCondition = "bShouldFloat"))
	float FloatHeight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (EditCondition = "bShouldFloat"))
	float FloatSpeed;

private:
	FVector InitialLocation;
	float RunningTime;
    
	void UpdatePickupMovement(float DeltaTime);
	void UpdateDestroyEffect(float DeltaTime);

	float DestroyTimer;
	bool bIsBeingDestroyed;
	float DestroyDuration;
	FVector OriginalScale;
	float OriginalParticleAlpha;
};
