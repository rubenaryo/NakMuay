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

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    void ResetCombatState();

    void QueueCombatAction(ECombatActionType actionIn);

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    void SetSkeletalMeshComponent(USkeletalMeshComponent* skMeshComp);

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    USkeletalMeshComponent* GetSkeletalMeshComponent() const { return SkeletalMeshComponent.Get(); }

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
    
    /**
     * Returns true if something was dequeued, false otherwise. 
     */
    UFUNCTION(BlueprintCallable)
    bool GetNextCombatActionInQueue(ECombatActionType& CombatActionType);

    UFUNCTION(BlueprintCallable)
    bool AbleToConsumeAction();

protected:
    ECombatActionType CurrentCombatActionType;
    
    TQueue<ECombatActionType> CombatActionQueue;
    
    TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;
};
