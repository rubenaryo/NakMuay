// (c) rubenaryo 2022

#pragma once

#include "CoreMinimal.h"
#include "CombatAction.h"

#include "Engine/HitResult.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CombatCommon.generated.h"

class AFighter;

class UPrimitiveComponent;

/// <summary>
/// Area on a fighter's body where a trace can occur for hit detection during an attack sequence
/// Keep 1:1 with CombatColliderSocketBindings
/// </summary>
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

/// <summary>
/// Can map to several bones. A gameplay abstraction of where a hit can occur.
/// </summary>
UENUM(BlueprintType, meta = (ScriptName = "HitSection"))
enum EHitSection
{
    HS_None,
    HS_Abdominal,
    HS_Head,
    HS_LeftLeg,
    HS_RightLeg,
    HS_Count
};

/// <summary>
/// Wrapper for common parameters used when a hit has occurred
/// </summary>
USTRUCT(BlueprintType)
struct FHitParameters
{
    GENERATED_BODY()

    FHitParameters() {}
    FHitParameters(const FVector& localPushImpulse,
        UPrimitiveComponent* hitComp,
        AFighter* attackingFighter,
        FHitResult hitResult,
        float physicsHitStrength,
        ECombatActionType attackType);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FVector LocalPushImpulse = FVector::ZeroVector;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UPrimitiveComponent* HitComp = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    AFighter* AttackingFighter = nullptr;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    FHitResult HitResult;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    float PhysicsHitStrength = 0.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    ECombatActionType AttackType = ECombatActionType::None;
};

/// <summary>
/// A dodge or movement type
/// </summary>
UENUM(BlueprintType, meta = (ScriptName = "MovementActionState"))
enum EMovementActionState
{
    MA_NONE         UMETA(DisplayName = "None"),
    MA_STRAFELEFT   UMETA(DisplayName = "Strafe Left"),
    MA_STRAFERIGHT  UMETA(DisplayName = "Strafe Right"),
    MA_COUNT,
};

UCLASS()
class NAKMUAY_API UCombatStatics : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    static const FName& GetSocketNameForColliderArea(ECombatColliderArea area);
};