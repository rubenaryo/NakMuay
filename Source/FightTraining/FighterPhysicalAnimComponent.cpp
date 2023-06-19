// (c) rubenaryo 2022
#include "FighterPhysicalAnimComponent.h"

FHitReactionJob::FHitReactionJob(const FVector& hitImpulse, const FVector& impactPoint, const FName& hitBoneName, float blendWeight)
    : HitImpulse(hitImpulse)
    , ImpactPoint(impactPoint)
    , HitBoneName(hitBoneName)
    , BlendWeight(blendWeight)
{
}

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

bool UFighterPhysicalAnimComponent::OnGetHit(UPrimitiveComponent* HitComp, AFighter* AttackingFighter, const FHitResult& InHitResult)
{
    return OnGetHit_Implementation(HitComp, AttackingFighter, InHitResult);
}

void UFighterPhysicalAnimComponent::SetActiveHitBlendWeight(FHitReactionJob& hitReactionJob, float newBlendWeight)
{
    hitReactionJob.BlendWeight = newBlendWeight;
}

void UFighterPhysicalAnimComponent::SortActiveHitsByBlendWeight()
{
    //class HitPredicate
    //{
    //public:
    //    bool operator()(const FHitReactionJob& A, const FHitReactionJob& B)
    //    {
    //        return A.BlendWeight < B.BlendWeight;
    //    }
    //} hitPredicate;
    //
    //ActiveHits.Sort(hitPredicate);
}

void UFighterPhysicalAnimComponent::PushActiveHit(const FHitReactionJob& hitReactionJob)
{
    ActiveHits.AddTail(hitReactionJob);
}

void UFighterPhysicalAnimComponent::TickActiveHits(float DeltaTime)
{
    using NodeType = TDoubleLinkedList<FHitReactionJob>::TDoubleLinkedListNode;

    uint8_t finishedHitReactionsCount = 0;
    
    NodeType* it = ActiveHits.GetHead();
    while (it)
    {
        FHitReactionJob& hitReaction = it->GetValue();
        bool bDone = TickSingleActiveHit_Implementation(DeltaTime, hitReaction);

        if (bDone)
        {
            finishedHitReactionsCount++;
        }

        it = it->GetNextNode();
    }

    for (uint8_t i = 0; i != finishedHitReactionsCount; ++i)
    {
        if (ActiveHits.GetHead())
            ActiveHits.RemoveNode(ActiveHits.GetHead());
    }
}
