#include "CombatActorComponent.h"

#include "Components/ShapeComponent.h"
#include "FighterPhysicalAnimComponent.h"
#include "Fighter.h"

const FName& UCombatActorComponent::GetSocketNameForColliderArea(ECombatColliderArea area)
{
    check(area < CCA_Count);
    return CombatColliderSocketBindings[area];
}

/////////////////////////
// Combat Actor Component

UCombatActorComponent::UCombatActorComponent()
    : CurrentCombatActionType(ECombatActionType::None)
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}

void UCombatActorComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UCombatActorComponent::GetNextCombatActionInQueue(ECombatActionType& CombatActionType)
{
    if (CombatActionQueue.IsEmpty())
        return false;

    return CombatActionQueue.Dequeue(CombatActionType);
}

bool UCombatActorComponent::AbleToConsumeAction()
{
    const bool bBufferedActions = !CombatActionQueue.IsEmpty();
    const bool bActionInProgress = CurrentCombatActionType != ECombatActionType::None;
    const bool bMoving = false;

    return bBufferedActions && !bActionInProgress && !bMoving;
}

bool UCombatActorComponent::OnGetHit(UPrimitiveComponent* HitComp, AFighter* AttackingFighter, const FHitResult& InHitResult, ECombatActionType AttackType, float PhysicsHitStrength)
{
    UFighterPhysicalAnimComponent* pPhysicalAnimComp = GetPhysicalAnimComponent();
    if (!pPhysicalAnimComp)
        return false;

    OnGetHit_BlueprintImpl(HitComp, AttackingFighter, InHitResult, AttackType, PhysicsHitStrength);

    return pPhysicalAnimComp->OnGetHit(HitComp, AttackingFighter, InHitResult, AttackType, PhysicsHitStrength);
}

void UCombatActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCombatActorComponent::SetCurrentCombatActionType(ECombatActionType combatAction)
{
    CurrentCombatActionType = combatAction;
    return true;
}

ECombatActionType UCombatActorComponent::GetCurrentCombatActionType() const
{
    return CurrentCombatActionType;
}

void UCombatActorComponent::ResetCombatState()
{
    // Currently, just resets to none.
    SetCurrentCombatActionType(ECombatActionType::None);
}

void UCombatActorComponent::QueueCombatAction(ECombatActionType actionIn)
{
    CombatActionQueue.Enqueue(actionIn);
}

void UCombatActorComponent::SetSkeletalMeshComponent(USkeletalMeshComponent* skMeshComp)
{
    SkeletalMeshComponent = skMeshComp;
}

void UCombatActorComponent::SetPhysicalAnimComponent(UFighterPhysicalAnimComponent* physicalAnimComp)
{
    PhysicalAnimComp = physicalAnimComp;
}

bool UCombatActorComponent::GetSocketTransformForColliderArea(FTransform& OutTransform, ECombatColliderArea colliderArea, ERelativeTransformSpace transformSpace)
{
    USkeletalMeshComponent* pSkelMeshComp = GetSkeletalMeshComponent();
    if (!pSkelMeshComp)
    {
        return false;
    }

    const FName& socketName = GetSocketNameForColliderArea(colliderArea);
    OutTransform = pSkelMeshComp->GetSocketTransform(socketName, transformSpace);
    return true;
}
