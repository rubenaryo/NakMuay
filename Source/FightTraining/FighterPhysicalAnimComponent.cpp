// (c) rubenaryo 2022


#include "FighterPhysicalAnimComponent.h"

UFighterPhysicalAnimComponent::UFighterPhysicalAnimComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFighterPhysicalAnimComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UFighterPhysicalAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    Tick_Implementation(DeltaTime);
}
