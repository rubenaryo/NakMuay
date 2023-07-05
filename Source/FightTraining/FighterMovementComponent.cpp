// (c) rubenaryo 2022


#include "FighterMovementComponent.h"

float UFighterMovementComponent::GetMaxSpeed() const
{
    if (OverrideMaxSpeed >= -0.9f)
        return OverrideMaxSpeed;

    return Super::GetMaxSpeed();
}
