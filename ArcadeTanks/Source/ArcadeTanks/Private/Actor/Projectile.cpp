// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Projectile.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "GameplayEffectTypes.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework//DamageType.h"
#include "Kismet//GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	RootComponent = ProjectileMesh;

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Smoke Trail"));
	TrailParticles->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	ProjectileMovementComponent->MaxSpeed = MaxSpeed;
	ProjectileMovementComponent->InitialSpeed = InitialSpeed;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
	}

	GetWorld()->GetTimerManager().SetTimer(
	DestroyTimerHandle,
	this,
	&AProjectile::OnLifeSpanExpired,
	LifeSpan,
	false
);
}

void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor);
        if (ASCInterface)
        {
            if (DamageEffectClass)
            {
                FGameplayEffectContextHandle EffectContext = ASCInterface->GetAbilitySystemComponent()->MakeEffectContext();
                EffectContext.AddSourceObject(this);

                FGameplayEffectSpecHandle SpecHandle = ASCInterface->GetAbilitySystemComponent()->MakeOutgoingSpec(DamageEffectClass, 1, EffectContext);
                if (SpecHandle.IsValid())
                {
                    SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), -Damage);
                    ASCInterface->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
                }
            }
        	HandleDestruction();
        }
        else if (!bHasBounced)
        {
        	if (Hit.Normal.Z > 0.8f)
        	{
        		HandleDestruction();
        		return;
        	}
        	
        	const FVector CurrentVelocity = ProjectileMovementComponent->Velocity;
        	const float Speed = CurrentVelocity.Size();
        	const FVector VelocityDir = CurrentVelocity.GetSafeNormal();
        	const FVector ReflectionDirection = FMath::GetReflectionVector(VelocityDir, Hit.Normal);
            
        	const FVector NewLocation = Hit.Location + (ReflectionDirection * 100.f);
        	SetActorLocation(NewLocation);
        	SetActorRotation(ReflectionDirection.Rotation());
            
        	UProjectileMovementComponent* NewProjectileMovement = NewObject<UProjectileMovementComponent>(this);
        	if (NewProjectileMovement)
        	{
        		NewProjectileMovement->RegisterComponent();
        		NewProjectileMovement->bRotationFollowsVelocity = true;
        		NewProjectileMovement->bShouldBounce = false;
        		NewProjectileMovement->ProjectileGravityScale = 0.0f;
        		NewProjectileMovement->SetUpdatedComponent(ProjectileMesh);
        		NewProjectileMovement->Velocity = ReflectionDirection * Speed;
                
        		if (ProjectileMovementComponent)
        		{
        			ProjectileMovementComponent->DestroyComponent();
        		}
                
        		ProjectileMovementComponent = NewProjectileMovement;
        	}

        	bHasBounced = true;
        }
        else
        {
	        HandleDestruction();
        }
	}
	else
	{
		HandleDestruction();
	}
}

void AProjectile::OnLifeSpanExpired()
{
	HandleDestruction();
	Destroy();
}

void AProjectile::HandleDestruction()
{
	if (HitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticles, GetActorLocation(), GetActorRotation());
	}
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	if (HitCameraShakeClass)
	{
		GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCameraShakeClass);
	}
	Destroy();
}
