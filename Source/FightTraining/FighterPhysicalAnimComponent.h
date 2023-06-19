// (c) rubenaryo 2022

#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "FighterPhysicalAnimComponent.generated.h"

class AFighter;

// Represents an active hit.
USTRUCT(BlueprintType)
struct FHitReactionJob
{
    GENERATED_BODY()

    FHitReactionJob() {}
    FHitReactionJob(const FVector& hitImpulse, const FVector& impactPoint, const FName& hitBoneName, float blendWeight);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector HitImpulse  = FVector::ZeroVector;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ImpactPoint = FVector::ZeroVector;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName HitBoneName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BlendWeight = 1.0f;
};

/**
 * Overrides stock physical anim component to generate procedural hit reactions.
 */
UCLASS(Blueprintable, ClassGroup = Combat, meta = (BlueprintSpawnableComponent))
class NAKMUAY_API UFighterPhysicalAnimComponent : public UPhysicalAnimationComponent
{
    GENERATED_BODY()

    UFighterPhysicalAnimComponent();

public:
    // Called when the game starts
    virtual void BeginPlay() override;

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, meta = (Tooltip = "Returns True if registered as a valid hit"))
    bool OnGetHit(UPrimitiveComponent* HitComp, AFighter* AttackingFighter, const FHitResult& InHitResult);

protected:
    UFUNCTION(BlueprintImplementableEvent)
    bool OnGetHit_Implementation(UPrimitiveComponent* HitComp, AFighter* AttackingFighter, const FHitResult& InHitResult);
    
    UFUNCTION(BlueprintImplementableEvent)
    void Tick_Implementation(float DeltaTime);

    UFUNCTION(BlueprintImplementableEvent, meta = (Tooltip = "Returns True if active hit has concluded blending. False otherwise."))
    bool TickSingleActiveHit_Implementation(float DeltaTime, UPARAM(ref) FHitReactionJob& InHitReactionRef);

    UFUNCTION(BlueprintCallable)
    void SetActiveHitBlendWeight(UPARAM(ref) FHitReactionJob& hitReactionJob, float newBlendWeight);

    UFUNCTION(BlueprintCallable)
    void SortActiveHitsByBlendWeight();

    UFUNCTION(BlueprintCallable)
    void PushActiveHit(const FHitReactionJob& hitReactionJob);

    UFUNCTION(BlueprintCallable)
    void TickActiveHits(float DeltaTime);

    // TODO: Maybe use a custom linked list container for these. For fast insert/delete
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    TDoubleLinkedList<FHitReactionJob> ActiveHits;
};
