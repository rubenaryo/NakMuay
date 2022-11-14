#include "CombatComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
    : CurrentCombatActionType(ECombatActionType::None)
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UCombatComponent::GetNextCombatActionInQueue(ECombatActionType& CombatActionType)
{
    if (CombatActionQueue.IsEmpty())
        return false;

    return CombatActionQueue.Dequeue(CombatActionType);
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCombatComponent::SetCurrentCombatActionType(ECombatActionType combatAction)
{
    CurrentCombatActionType = combatAction;
    return true;
}

ECombatActionType UCombatComponent::GetCurrentCombatActionType() const
{
    return CurrentCombatActionType;
}

void UCombatComponent::ResetCombatState()
{
    // Currently, just resets to none.
    SetCurrentCombatActionType(ECombatActionType::None);
}

void UCombatComponent::QueueCombatAction(ECombatActionType actionIn)
{
    CombatActionQueue.Enqueue(actionIn);
}

void UCombatComponent::SetSkeletalMeshComponent(USkeletalMeshComponent* skMeshComp)
{
    SkeletalMeshComponent = skMeshComp;
}
