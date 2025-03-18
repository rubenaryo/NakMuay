// (c) rubenaryo 2022

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifySnapToTarget.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "AN_SnapToTarget", Tooltip = "When triggered, will attempt to snap to the fighter's lock on target.", Category = "Animation|Notifies"))
class NAKMUAY_API UAnimNotifySnapToTarget : public UAnimNotifyState
{
	GENERATED_BODY()

private: 
	bool bSnapEnabled = false;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters", meta = (UIMin = "100.0", UIMax = "1500.0", SliderExponent = "5.0"))
    float SnapRange = 400.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters", meta = (UIMin = "0.0", UIMax = "150.0", SliderExponent = "1.0"))
    float SnapDistance = 75.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Parameters", meta = (UIMin = "0.01", UIMax = "20.0", SliderExponent = "0.1"))
    float SnapSpeed = 6.0f;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
