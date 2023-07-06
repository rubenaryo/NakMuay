#include "CombatCommon.h"

//////////////////////////////
// FHitParameters

FHitParameters::FHitParameters(const FVector& localPushImpulse, UPrimitiveComponent* hitComp, AFighter* attackingFighter, FHitResult hitResult, float physicsHitStrength, ECombatActionType attackType)
    : LocalPushImpulse(localPushImpulse)
    , HitComp(hitComp)
    , AttackingFighter(attackingFighter)
    , HitResult(hitResult)
    , PhysicsHitStrength(physicsHitStrength)
    , AttackType(attackType)
{
}

const FName& UCombatStatics::GetSocketNameForColliderArea(ECombatColliderArea area)
{
    check(area < CCA_Count);
    return CombatColliderSocketBindings[area];
}