
#include "HitReactionMap.h"

#include "Math/Vector.h"
#include "UObject/ObjectSaveContext.h"

const UAnimMontage* UHitReactionMap::GetMontageFromHitSectionAndImpulse(EHitSection hitSection, const FVector& Impulse)
{
    FHitReactionReferenceArray* hitSectionResult = HitSectionToReactionMap.Find(hitSection);
    if (!hitSectionResult)
        return nullptr;

    double maxDotProduct = -DBL_MAX;
    const UAnimMontage* retVal = nullptr;

    const TArray<FHitReactionReferenceData>& hitReactionReferenceArray = hitSectionResult->ReactionReferenceArray;
    int32_t numRefVectors = hitReactionReferenceArray.Num();
    for (int32_t i = 0; i != numRefVectors; ++i)
    {
        const FHitReactionReferenceData& reactionReferenceData = hitReactionReferenceArray[i];

        double dotProduct = FVector::DotProduct(Impulse, reactionReferenceData.ReferenceVector);
        if (dotProduct > maxDotProduct)
        {
            maxDotProduct = dotProduct;
            retVal = reactionReferenceData.Montage;
        }
    }

    return retVal;
}

EHitSection UHitReactionMap::GetHitSectionFromBoneName(const FName& boneName)
{
    TEnumAsByte<EHitSection>* hitSectionResult = BoneToHitSectionMap.Find(boneName);
    if (!hitSectionResult)
        return EHitSection::HS_None;

    return *hitSectionResult;
}

UHitReactionMap::UHitReactionMap() : Super()
{

}

void UHitReactionMap::PreSave(FObjectPreSaveContext ObjectSaveContext)
{
    Super::PreSave(ObjectSaveContext);

    BoneToHitSectionMap.Empty();
    for (const auto& pair : HitSectionToBoneMap)
    {
        EHitSection hitSection = pair.Key;
        const TArray<FName>& bonenameArray = pair.Value.BoneNames;

        for (int32_t i = 0; i != bonenameArray.Num(); ++i)
        {
            BoneToHitSectionMap.Add(bonenameArray[i], hitSection);
        }
    }
}
