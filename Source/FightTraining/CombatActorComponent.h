// Fil out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAction.h"
#include "CombatCommon.h"
#include "Components/ActorComponent.h"
#include "CombatActorComponent.generated.h"

class AFighter;
class UHitReactionMap;
class UFighterPhysicalAnimComponent;

class UCharacterMovementComponent;
class USkeletalMeshComponent;

UCLASS(Blueprintable, ClassGroup = Combat, meta=(BlueprintSpawnableComponent))
class NAKMUAY_API UCombatActorComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UCombatActorComponent();

public:	
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    bool SetCurrentCombatActionType(ECombatActionType combatAction);

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    ECombatActionType GetCurrentCombatActionType() const;

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    void ResetCombatState();

    void QueueCombatAction(ECombatActionType actionIn);

    // Component Reference Setters/Getters
    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    void SetMovementComponent(UCharacterMovementComponent* moveComp);
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    void SetSkeletalMeshComponent(USkeletalMeshComponent* skMeshComp);
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    void SetPhysicalAnimComponent(UFighterPhysicalAnimComponent* physicalAnimComp);

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    UCharacterMovementComponent* GetMovementComponent() const { return MovementComponent.Get(); }

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    USkeletalMeshComponent* GetSkeletalMeshComponent() const { return SkeletalMeshComponent.Get(); }

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    UFighterPhysicalAnimComponent* GetPhysicalAnimComponent() const { return PhysicalAnimComp.Get(); }


    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    bool GetSocketTransformForColliderArea(FTransform& OutTransform, ECombatColliderArea colliderArea, ERelativeTransformSpace transformSpace = ERelativeTransformSpace::RTS_World);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HandleActiveAttackBegin(ECombatColliderArea colliderArea, float PhysicsHitStrength);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HandleActiveAttackTick(ECombatColliderArea colliderArea, float TraceRadius, float PhysicsHitStrength, const FVector& LocalPushImpulse);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HandleActiveAttackEnd(ECombatColliderArea colliderArea, float PhysicsHitStrength);

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

    UFUNCTION(BlueprintCallable, meta = (Tooltip = "Returns True if registered as a valid hit"))
    bool OnGetHit(const FHitParameters& hitParameters);

    UFUNCTION(BlueprintImplementableEvent, meta = (Tooltip = "Returns True if registered as a valid hit"))
    bool OnGetHit_Implementation(const FHitParameters& hitParameters);

    UFUNCTION(BlueprintCallable)
    void SetRegisteredAttackHitFlag(ECombatColliderArea cca, bool bEnable)
    {
        uint32_t flag = 1 << cca;
        if (bEnable)
        {
            RegisteredAttackHitFlags |= flag;
        }
        else
        {
            RegisteredAttackHitFlags &= ~flag;
        }
    }

    UFUNCTION(BlueprintCallable)
    bool IsRegisteredAttackHit(ECombatColliderArea cca)
    {
        uint32_t flag = 1 << cca;
        return (RegisteredAttackHitFlags & flag) != 0;
    }

protected:
    ECombatActionType CurrentCombatActionType;
    
    TQueue<ECombatActionType> CombatActionQueue;
    
    TWeakObjectPtr<UCharacterMovementComponent> MovementComponent;

    TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    TWeakObjectPtr<UFighterPhysicalAnimComponent> PhysicalAnimComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    const TObjectPtr<UHitReactionMap> HitReactionMapAsset;

    uint32_t RegisteredAttackHitFlags; // Records ECombatColliderArea as flags for which attack is active or not.
};
