#pragma once

#include "CoreMinimal.h"
#include "CombatCommon.h"
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

    UHitReactionMap();

    // Populates the hit section maps.
    virtual void PreSave(FObjectPreSaveContext ObjectSaveContext) override;

public:
    UFUNCTION(BlueprintCallable)
    const UAnimMontage* GetMontageFromHitSectionAndImpulse(EHitSection hitSection, const FVector& Impulse);

    UFUNCTION(BlueprintCallable)
    EHitSection GetHitSectionFromBoneName(const FName& boneName);

private:
    /// <summary>
    /// Maps a body's hit section with the bones it represents
    /// </summary>
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Hit Section to Bone Map", AllowPrivateAccess = "true"))
    TMap<TEnumAsByte<EHitSection>, FBoneNameArray> HitSectionToBoneMap;

    /// <summary>
    /// Opposite map to HitSectionToBoneMap. For a given bone, returns its hit section. 
    /// </summary>
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (DisplayName = "Bone to Hit Section Map (generated)", AllowPrivateAccess = "true"))
    TMap<FName, TEnumAsByte<EHitSection>> BoneToHitSectionMap;
    
    /// <summary>
    /// For a given hit section, returns the reactions that can play from tuned reference vectors.
    /// </summary>
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (DisplayName = "Hit Section to Reaction Map", AllowPrivateAccess = "true"))
    TMap<TEnumAsByte<EHitSection>, FHitReactionReferenceArray> HitSectionToReactionMap;

};