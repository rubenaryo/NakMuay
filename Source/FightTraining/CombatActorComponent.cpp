#include "CombatActorComponent.h"

#include "Components/ShapeComponent.h"
#include "FighterPhysicalAnimComponent.h"
#include "Fighter.h"

const FName& GetSocketNameForColliderArea(ECombatColliderArea area)
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

void UCombatActorComponent::SetColliderShape(UShapeComponent* colliderShape, int index)
{
    if (index < CCA_Count)
    {
        colliderShape->SetCollisionObjectType(ECC_PhysicsBody);
        colliderShape->OnComponentHit.AddDynamic(this, &UCombatActorComponent::OnHitCallback);
        colliderShape->OnComponentBeginOverlap.AddDynamic(this, &UCombatActorComponent::OnOverlapCallback);
        CombatColliderShapes[index] = colliderShape;
    }
}

void UCombatActorComponent::BeginPlay()
{
    Super::BeginPlay();

    static const FAttachmentTransformRules skAttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

    for (uint32_t i = 0; i != CCA_Count; ++i)
    {
        ECombatColliderArea cca = (ECombatColliderArea)i;
        UShapeComponent* pPrim = GetCollider(cca);

        if (pPrim)
        {
            const FName& socketName = GetSocketNameForColliderArea(cca);
            if (!pPrim->AttachToComponent(GetSkeletalMeshComponent(), skAttachmentRules, socketName))
            {
                GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, socketName.GetPlainNameString());
            }

            pPrim->SetVisibility(true);
            pPrim->SetHiddenInGame(false);
        }
    }
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

void UCombatActorComponent::OnHitCallback(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    AFighter* pOtherFighter = Cast<AFighter>(OtherActor);
    if (!pOtherFighter || OtherActor->GetUniqueID() == GetOwner()->GetUniqueID())
        return;

    UFighterPhysicalAnimComponent* pOtherPhysicalAnimComp = pOtherFighter->GetPhysicalAnimComponent();

    OnHit_BlueprintImpl(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

    pOtherPhysicalAnimComp->OnHit_Implementation(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
}

void UCombatActorComponent::OnOverlapCallback(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    AFighter* pOtherFighter = Cast<AFighter>(Other);
    if (!pOtherFighter || Other->GetUniqueID() == GetOwner()->GetUniqueID())
        return;

    int stub = 42;
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

void UCombatActorComponent::EnableCollider(ECombatColliderArea colliderArea, bool bEnable)
{
    if (colliderArea >= CCA_Count)
        return;

     UShapeComponent* collider = GetCollider(colliderArea);
    if (!collider)
        return;

    if (bEnable)
    {
        collider->SetNotifyRigidBodyCollision(true);
        collider->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
    }
    else
    {
        collider->SetNotifyRigidBodyCollision(false);
        collider->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
    }
}
