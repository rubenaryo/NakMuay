// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatAction.h"
#include "Components/ActorComponent.h"
#include "CombatActorComponent.generated.h"

class AFighter;
class USkeletalMeshComponent;
class UFighterPhysicalAnimComponent;

/** 1:1 with ColliderSocketBindings >:3 **/
UENUM(BlueprintType, meta = (ScriptName = "CombatColliderArea"))
enum ECombatColliderArea
{
    CCA_LeftHand,
    CCA_RightHand,
    CCA_LeftKnee,
    CCA_RightKnee,
    CCA_LeftShin,
    CCA_RightShin,
    CCA_LeftFoot,
    CCA_RightFoot,
    CCA_LeftElbow,
    CCA_RightElbow,

    CCA_Count
};

inline FName CombatColliderSocketBindings[CCA_Count] =
{
    FName(TEXT("hand_l_Socket")),	// CCA_LeftHand
    FName(TEXT("hand_r_Socket")),	// CCA_RightHand
    FName(TEXT("knee_l_Socket")),	// CCA_LeftKnee
    FName(TEXT("knee_r_Socket")),	// CCA_RightKnee
    FName(TEXT("shin_l_Socket")),	// CCA_LeftShin
    FName(TEXT("shin_r_Socket")),	// CCA_RightShin
    FName(TEXT("foot_l_Socket")),	// CCA_LeftFoot
    FName(TEXT("foot_r_Socket")),	// CCA_RightFoot
    FName(TEXT("elbow_l_Socket")),	// CCA_LeftElbow
    FName(TEXT("elbow_r_Socket"))	// CCA_RightElbow
};


UENUM(BlueprintType, meta = (ScriptName = "MovementActionState"))
enum EMovementActionState
{
    MA_NONE         UMETA(DisplayName="None"),
    MA_STRAFELEFT   UMETA(DisplayName="Strafe Left"),
    MA_STRAFERIGHT  UMETA(DisplayName="Strafe Right"),
    MA_COUNT,
};

UCLASS(Blueprintable, ClassGroup = Combat, meta=(BlueprintSpawnableComponent))
class NAKMUAY_API UCombatActorComponent : public UActorComponent
{
    GENERATED_BODY()

public:	
    // Sets default values for this component's properties
    UCombatActorComponent();

    UFUNCTION(BlueprintCallable)
    static const FName& GetSocketNameForColliderArea(ECombatColliderArea area);
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

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    UFighterPhysicalAnimComponent* GetPhysicalAnimComponent() const { return PhysicalAnimComp.Get(); }
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    void SetPhysicalAnimComponent(UFighterPhysicalAnimComponent* physicalAnimComp);

    UFUNCTION(BlueprintCallable, Category = "Combat|Components|CombatComponent")
    bool GetSocketTransformForColliderArea(FTransform& OutTransform, ECombatColliderArea colliderArea, ERelativeTransformSpace transformSpace = ERelativeTransformSpace::RTS_World);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HandleActiveAttackBegin(ECombatColliderArea colliderArea);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HandleActiveAttackTick(ECombatColliderArea colliderArea, float TraceRadius);

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void HandleActiveAttackEnd(ECombatColliderArea colliderArea);

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
    bool OnGetHit(UPrimitiveComponent* HitComp, AFighter* AttackingFighter, const FHitResult& InHitResult);

    UFUNCTION(BlueprintImplementableEvent, meta = (Tooltip = "Returns True if registered as a valid hit"))
    bool OnGetHit_BlueprintImpl(UPrimitiveComponent* HitComp, AFighter* AttackingFighter, const FHitResult& InHitResult);

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
    
    TWeakObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

    TWeakObjectPtr<UFighterPhysicalAnimComponent> PhysicalAnimComp;

    uint32_t RegisteredAttackHitFlags; // Records ECombatColliderArea as flags for which attack is active or not.
};
