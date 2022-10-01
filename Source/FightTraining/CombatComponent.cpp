#include "CombatComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
    : CurrentCombatAction(CA_NONE)
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

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UCombatComponent::IsPunching() const
{
    return GetCurrentCombatAction() == CA_PUNCH;
}

void UCombatComponent::Punch()
{
    // Currently, just sets a variable which can be used for logic in the Animation Blueprints.
    SetCurrentCombatAction(CA_PUNCH);
}

bool UCombatComponent::SetCurrentCombatAction(ECombatActionState combatAction)
{
    CurrentCombatAction = combatAction;
    return true;
}

ECombatActionState UCombatComponent::GetCurrentCombatAction() const
{
    return CurrentCombatAction;
}

void UCombatComponent::ResetCombatState()
{
    // Currently, just resets to none.
    SetCurrentCombatAction(CA_NONE);
}
