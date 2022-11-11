// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAction.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "MovementActionState"))
enum EMovementActionState
{
    MA_NONE         UMETA(DisplayName="None"),
    MA_STRAFELEFT   UMETA(DisplayName="Strafe Left"),
    MA_STRAFERIGHT  UMETA(DisplayName="Strafe Right"),
    MA_COUNT,
};

UCLASS(Blueprintable, ClassGroup = Combat, meta=(BlueprintSpawnableComponent))
class FIGHTTRAINING_API UCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UCombatComponent();

public:	
    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    bool SetCurrentCombatActionType(ECombatActionType combatAction);

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    ECombatActionType GetCurrentCombatActionType() const;

    void ResetCombatState();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

protected:
    ECombatActionType CurrentCombatActionType;
};
