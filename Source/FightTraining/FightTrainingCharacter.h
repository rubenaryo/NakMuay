// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FightTrainingCharacter.generated.h"

#define RY_TOUCHCONTROLS 0

class UCameraComponent;
class UCombatComponent;
class USphereComponent;
class USpringArmComponent;

UCLASS(config=Game)
class AFightTrainingCharacter : public ACharacter
{
    GENERATED_BODY()

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    /** Core Combat Component **/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    UCombatComponent* CombatComponent;
    
    /** Core Combat Component **/
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
    USphereComponent* LeftFistCollider;

public:
    AFightTrainingCharacter();

    /** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input)
    float TurnRateGamepad;

protected:

    void BeginPlay() override;

    /** Called for forwards/backward input */
    void MoveForward(float Value);

    /** Called for side to side input */
    void MoveRight(float Value);

    /** 
     * Called via input to turn at a given rate. 
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void TurnAtRate(float Rate);

    /**
     * Called via input to turn look up/down at a given rate. 
     * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
     */
    void LookUpAtRate(float Rate);

    // These should probably be somewhere other than the main character file
    void PunchAction();
    void ResetCombat();

#if RY_TOUCHCONTROLS
    /** Handler for when a touch input begins. */
    void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

    /** Handler for when a touch input stops. */
    void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);
#endif

protected:
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // End of APawn interface

public:
    /** Returns CameraBoom subobject **/
    FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
    /** Returns CombatComponent subobject **/
    FORCEINLINE UCombatComponent* GetCombatComponent() const { return CombatComponent; }
};

