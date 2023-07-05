
#include "HitReactionMap.h"
#include "Math/Vector.h"

const UAnimMontage* UHitReactionMap::GetMontageFromBoneNameAndImpulse(const FName& boneName, const FVector& Impulse)
{
    FHitReactionReferenceArray* boneNameResult = BoneToHitReactionMap.Find(boneName);
    if (!boneNameResult)
        return nullptr;

    double maxDotProduct = -DBL_MAX;
    const UAnimMontage* retVal = nullptr;

    const TArray<FHitReactionReferenceData>& hitReactionReferenceArray = boneNameResult->ReactionReferenceArray;
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
