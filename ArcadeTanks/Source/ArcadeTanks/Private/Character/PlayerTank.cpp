// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerTank.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APlayerTank::APlayerTank()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());  // Attach to capsule
	SpringArm->TargetArmLength = SpringArmLength;
	SpringArm->SetRelativeRotation(CameraRotation);
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraLagSpeed = 3.0f;
	SpringArm->CameraRotationLagSpeed = 3.0f;
	SpringArm->CameraLagMaxDistance = 100.0f;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

void APlayerTank::BeginPlay()
{
	Super::BeginPlay();
}
