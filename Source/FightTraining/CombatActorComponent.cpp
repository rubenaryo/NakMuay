#include "CombatActorComponent.h"

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
