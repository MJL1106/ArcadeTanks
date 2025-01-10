// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ArcadeTanksPlayerController.h"

#include "Character/TankCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

AArcadeTanksPlayerController::AArcadeTanksPlayerController()
{
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AArcadeTanksPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Get reference to controlled tank
    ControlledTank = Cast<ATankCharacterBase>(GetPawn());

    // Add Input Mapping Context
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(DefaultMappingContext, 0);
    }
}

void AArcadeTanksPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        // Bind the actions
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AArcadeTanksPlayerController::HandleMoveInput);
        EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AArcadeTanksPlayerController::HandleRotateInput);
    }
}

void AArcadeTanksPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ControlledTank)
    {
        HandleTurretRotation();
    }
}

void AArcadeTanksPlayerController::HandleMoveInput(const FInputActionValue& Value)
{
    const float MovementValue = Value.Get<float>();
    
    if (ControlledTank)
    {
        const FVector Direction = ControlledTank->GetActorForwardVector();
        ControlledTank->AddMovementInput(Direction, MovementValue);
    }
}

void AArcadeTanksPlayerController::HandleRotateInput(const FInputActionValue& Value)
{
    const float RotationValue = Value.Get<float>();
    
    if (ControlledTank)
    {
        const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
        FRotator DeltaRotation = FRotator::ZeroRotator;
        DeltaRotation.Yaw = RotationValue * ControlledTank->GetTurnRate() * DeltaTime;
        ControlledTank->AddActorLocalRotation(DeltaRotation, true);
    }
}

void AArcadeTanksPlayerController::HandleTurretRotation()
{
    if (ControlledTank)
    {
        FVector MouseLocation = GetMouseWorldLocation();
        ControlledTank->RotateTurret(MouseLocation);
    }
}

FVector AArcadeTanksPlayerController::GetMouseWorldLocation() const
{
    FVector WorldLocation, WorldDirection;
    if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
    {
        FVector StartLocation = WorldLocation;
        FVector EndLocation = WorldLocation + (WorldDirection * 8000.0f);

        FHitResult HitResult;
        if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility))
        {
            return HitResult.Location;
        }
        return EndLocation;
    }
    return FVector::ZeroVector;
}

void AArcadeTanksPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
    if (bPlayerEnabled)
    {
        GetPawn()->EnableInput(this);
    }
    else
    {
        GetPawn()->DisableInput(this);
    }
    bShowMouseCursor = bPlayerEnabled;
}
