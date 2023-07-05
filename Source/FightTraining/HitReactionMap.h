#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "HitReactionMap.generated.h"

USTRUCT(BlueprintType)
struct FHitReactionReferenceData
{
    GENERATED_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector ReferenceVector;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UAnimMontage> Montage;
};

USTRUCT(BlueprintType)
struct FHitReactionReferenceArray
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FHitReactionReferenceData> ReactionReferenceArray;
};

UCLASS(Blueprintable, ClassGroup = Combat)
class UHitReactionMap : public UDataAsset
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    const UAnimMontage* GetMontageFromBoneNameAndImpulse(const FName& boneName, const FVector& Impulse);

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    TMap<FName, FHitReactionReferenceArray> BoneToHitReactionMap;
};
