// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "CombatActionState"))
enum ECombatActionState
{
    CA_NONE     UMETA(DisplayName="None"),
    CA_PUNCH    UMETA(DisplayName="Punch"),
    CA_COUNT,
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
    bool IsPunching() const;

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    void Punch();

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    bool SetCurrentCombatAction(ECombatActionState combatAction);

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    ECombatActionState GetCurrentCombatAction() const;

    void ResetCombatState();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;


protected:
    ECombatActionState CurrentCombatAction;
};
