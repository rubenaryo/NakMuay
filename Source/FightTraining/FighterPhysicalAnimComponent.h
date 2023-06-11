// (c) rubenaryo 2022

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "FighterPhysicalAnimComponent.generated.h"

/**
 * 
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

    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void OnHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
    UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
    void Tick_Implementation(float DeltaTime);
};
