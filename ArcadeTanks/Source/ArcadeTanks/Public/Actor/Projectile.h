// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class ARCADETANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float MaxSpeed = 1300.f;

	UPROPERTY(EditAnywhere)
	float InitialSpeed = 1300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	float Damage  = 20.f;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UStaticMeshComponent* ProjectileMesh;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	TSubclassOf<class UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* HitParticles;
	
	UPROPERTY(VisibleAnywhere, Category = "Combat")
	UParticleSystemComponent* TrailParticles;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LaunchSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UCameraShakeBase> HitCameraShakeClass;
	
	UPROPERTY()
	bool bHasBounced = false;
	
	UPROPERTY()
	FTimerHandle DestroyTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float LifeSpan = 10.f;
	
	UFUNCTION()
	void OnLifeSpanExpired();

	void HandleDestruction();
	

};
