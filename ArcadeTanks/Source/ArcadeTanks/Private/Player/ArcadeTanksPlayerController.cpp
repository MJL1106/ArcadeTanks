// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ArcadeTanksPlayerController.h"

#include "AbilitySystemComponent.h"
#include "Character/TankCharacterBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "TankGameplayTags.h"
#include "AbilitySystem/TankAbilitySystemComponent.h"
#include "AbilitySystem/TankAttributeSet.h"
#include "Blueprint/UserWidget.h"
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

    if (MobileControlsWidgetClass)
    {
        MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);
        if (MobileControlsWidget)
        {
            MobileControlsWidget->AddToViewport();
        }
    }
}

void AArcadeTanksPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
    {
        // Bind the actions
        /*
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AArcadeTanksPlayerController::HandleMoveInput);
        EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &AArcadeTanksPlayerController::HandleRotateInput);
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AArcadeTanksPlayerController::HandleShootInput);
        */

        /*EnhancedInputComponent->BindAction(TouchMoveAction, ETriggerEvent::Triggered, this, &AArcadeTanksPlayerController::HandleTouchMoveTriggered);
        EnhancedInputComponent->BindAction(TouchAimAction, ETriggerEvent::Triggered, this, &AArcadeTanksPlayerController::HandleTouchAimTriggered);
        EnhancedInputComponent->BindAction(TouchShootAction, ETriggerEvent::Started, this, &AArcadeTanksPlayerController::HandleTouchShootTriggered);*/
    }
    
    InputComponent->BindAxis("MoveForward", this, &AArcadeTanksPlayerController::HandleMoveForwardAxis);
    InputComponent->BindAxis("Turn", this, &AArcadeTanksPlayerController::HandleTurnAxis);
    InputComponent->BindAxis("RotateTurret", this, &AArcadeTanksPlayerController::HandleRotateTurretAxis);
    InputComponent->BindAxis("RotateTurretY", this, &AArcadeTanksPlayerController::HandleRotateTurretYAxis);
}

bool AArcadeTanksPlayerController::InputTouch(uint32 Handle, ETouchType::Type Type, const FVector2D& TouchLocation, float Force, FDateTime DeviceTimestamp, uint32 TouchpadIndex)
{
    Super::InputTouch(Handle, Type, TouchLocation, Force, DeviceTimestamp, TouchpadIndex);
    return false;
}

void AArcadeTanksPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (ControlledTank)
    {
        // Check if right joystick is being used
        if (RightJoystickInput.Size() > 0.1f)  // Deadzone
        {
            // Aim turret based on joystick direction
            FVector TankLocation = ControlledTank->GetActorLocation();
            
            // FIXED: Inverted Y so down = forward
            FVector AimDirection = ControlledTank->GetActorForwardVector() * (-RightJoystickInput.Y) + 
                                   ControlledTank->GetActorRightVector() * RightJoystickInput.X;
            FVector AimTarget = TankLocation + (AimDirection * 1000.0f);
            
            ControlledTank->RotateTurret(AimTarget);
        }
        // REMOVED the else block - turret stays where it is when joystick released
    }
}

void AArcadeTanksPlayerController::MoveForward(float Value)
{
    if (ControlledTank && Value != 0.0f)
    {
        FVector Direction = ControlledTank->GetActorForwardVector();
        ControlledTank->AddMovementInput(Direction, Value);
    }
}

void AArcadeTanksPlayerController::MoveRight(float Value)
{
    if (ControlledTank && Value != 0.0f)
    {
        const float DeltaTime = GetWorld()->GetDeltaSeconds();
        FRotator DeltaRotation = FRotator::ZeroRotator;
        DeltaRotation.Yaw = Value * ControlledTank->GetTurnRate() * DeltaTime;
        ControlledTank->AddActorLocalRotation(DeltaRotation, true);
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

void AArcadeTanksPlayerController::HandleShootInput()
{
    UTankAbilitySystemComponent* TankASC = Cast<UTankAbilitySystemComponent>(ControlledTank->GetAbilitySystemComponent());
    
    if (ControlledTank && TankASC)
    {
        FGameplayTag EventTag = FTankGameplayTags::Get().Ability_Tank_Shoot;
        
        TankASC->TryActivateAbilitiesByTag(
            FGameplayTagContainer(EventTag));
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

void AArcadeTanksPlayerController::HandleTouchMoveTriggered(const FInputActionValue& Value)
{
    const FVector2D TouchValue = Value.Get<FVector2D>();
    
    if (ControlledTank)
    {
        // Get screen size
        int32 ViewportSizeX, ViewportSizeY;
        GetViewportSize(ViewportSizeX, ViewportSizeY);
        
        // Check if touch is on left half of screen (movement area)
        if (TouchValue.X < ViewportSizeX * 0.5f)
        {
            // Convert touch to movement direction (virtual joystick style)
            FVector2D JoystickCenter = FVector2D(ViewportSizeX * 0.25f, ViewportSizeY * 0.75f);
            FVector2D JoystickOffset = TouchValue - JoystickCenter;
            
            // Normalize and clamp
            float Distance = JoystickOffset.Size();
            float MaxDistance = 100.0f; // Joystick radius in pixels
            
            if (Distance > 1.0f)
            {
                FVector2D Direction = JoystickOffset / Distance;
                float Magnitude = FMath::Clamp(Distance / MaxDistance, 0.0f, 1.0f);
                
                // Apply movement
                FVector ForwardDir = ControlledTank->GetActorForwardVector();
                ControlledTank->AddMovementInput(ForwardDir, Direction.Y * Magnitude);
                
                // Apply rotation
                const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
                FRotator DeltaRotation = FRotator::ZeroRotator;
                DeltaRotation.Yaw = Direction.X * ControlledTank->GetTurnRate() * DeltaTime * Magnitude;
                ControlledTank->AddActorLocalRotation(DeltaRotation, true);
            }
        }
    }
}

void AArcadeTanksPlayerController::HandleTouchAimTriggered(const FInputActionValue& Value)
{
    const FVector2D TouchValue = Value.Get<FVector2D>();
    
    if (ControlledTank)
    {
        // Get screen size
        int32 ViewportSizeX, ViewportSizeY;
        GetViewportSize(ViewportSizeX, ViewportSizeY);
        
        // Check if touch is on right half of screen (aiming area)
        if (TouchValue.X > ViewportSizeX * 0.5f)
        {
            // Deproject touch to world location
            FVector WorldLocation, WorldDirection;
            if (DeprojectScreenPositionToWorld(TouchValue.X, TouchValue.Y, WorldLocation, WorldDirection))
            {
                FVector StartLocation = WorldLocation;
                FVector EndLocation = WorldLocation + (WorldDirection * 8000.0f);

                FHitResult HitResult;
                if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility))
                {
                    TouchAimWorldLocation = HitResult.Location;
                    bIsTouchAiming = true;
                }
            }
        }
    }
}

void AArcadeTanksPlayerController::HandleTouchShootTriggered(const FInputActionValue& Value)
{
    // Reuse existing shoot logic
    HandleShootInput();
}

void AArcadeTanksPlayerController::HandleMoveForwardAxis(float Value)
{
    if (ControlledTank && FMath::Abs(Value) > 0.1f)  // Deadzone
    {
        FVector Direction = ControlledTank->GetActorForwardVector();
        ControlledTank->AddMovementInput(Direction, Value);
    }
}

void AArcadeTanksPlayerController::HandleTurnAxis(float Value)
{
    if (ControlledTank && FMath::Abs(Value) > 0.1f)  // Deadzone
    {
        const float DeltaTime = GetWorld()->GetDeltaSeconds();
        FRotator DeltaRotation = FRotator::ZeroRotator;
        DeltaRotation.Yaw = Value * ControlledTank->GetTurnRate() * DeltaTime;
        ControlledTank->AddActorLocalRotation(DeltaRotation, true);
    }
}

void AArcadeTanksPlayerController::HandleRotateTurretAxis(float Value)
{
    RightJoystickInput.X = Value;
}

void AArcadeTanksPlayerController::HandleRotateTurretYAxis(float Value)
{
    RightJoystickInput.Y = Value;
}
