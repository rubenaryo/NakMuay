// Copyright Epic Games, Inc. All Rights Reserved.

#include "FightTrainingCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "FightTraining/CombatComponent.h"

//////////////////////////////////////////////////////////////////////////
// AFightTrainingCharacter

AFightTrainingCharacter::AFightTrainingCharacter()
{
    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
    // are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFightTrainingCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    // Set up gameplay key bindings
    check(PlayerInputComponent);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

    PlayerInputComponent->BindAction("Punch", IE_Pressed, this, &AFightTrainingCharacter::PunchAction);
    PlayerInputComponent->BindAction("Punch", IE_Released, this, &AFightTrainingCharacter::ResetCombat);
    
    PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFightTrainingCharacter::MoveForward);
    PlayerInputComponent->BindAxis("Move Right / Left", this, &AFightTrainingCharacter::MoveRight);

    // We have 2 versions of the rotation bindings to handle different kinds of devices differently
    // "turn" handles devices that provide an absolute delta, such as a mouse.
    // "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
    PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
    PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AFightTrainingCharacter::TurnAtRate);
    PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AFightTrainingCharacter::LookUpAtRate);

#if RY_TOUCHCONTROLS
    // handle touch devices
    PlayerInputComponent->BindTouch(IE_Pressed, this, &AFightTrainingCharacter::TouchStarted);
    PlayerInputComponent->BindTouch(IE_Released, this, &AFightTrainingCharacter::TouchStopped);
#endif
}

#if RY_TOUCHCONTROLS
void AFightTrainingCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
    Jump();
}

void AFightTrainingCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
    StopJumping();
}
#endif

void AFightTrainingCharacter::TurnAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFightTrainingCharacter::LookUpAtRate(float Rate)
{
    // calculate delta for this frame from the rate information
    AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AFightTrainingCharacter::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void AFightTrainingCharacter::MoveRight(float Value)
{
    if ( (Controller != nullptr) && (Value != 0.0f) )
    {
        // find out which way is right
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
    
        // get right vector 
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        // add movement in that direction
        AddMovementInput(Direction, Value);
    }
}

void AFightTrainingCharacter::PunchAction()
{
    CombatComponent->Punch();
}

void AFightTrainingCharacter::ResetCombat()
{
    CombatComponent->ResetCombatState();
}