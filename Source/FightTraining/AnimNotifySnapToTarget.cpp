// (c) rubenaryo 2022


#include "AnimNotifySnapToTarget.h"
#include "Fighter.h"
#include "Math/UnrealMathUtility.h"

void UAnimNotifySnapToTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
    // Reset cached values
    bSnapEnabled = false;

    // Get owner actor and try to cast to Fighter
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimNotify: No valid owner found"));
        return;
    }

    // Cast to Fighter
    AFighter* Fighter = Cast<AFighter>(Owner);
    if (!Fighter)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimNotify: Owner is not a Fighter"));
        return;
    }

    // Get the lock-on target
    AActor* LockOnTarget = Fighter->GetLockOnTarget(); // Replace with your actual getter method name
    if (!LockOnTarget)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimNotify: No valid lock-on target found"));
        return;
    }

    // Get current positions
    FVector CurrentLocation = Fighter->GetActorLocation();
    FVector TargetLocation = LockOnTarget->GetActorLocation();

    FVector diff = TargetLocation - CurrentLocation;
    float sqDist = diff.SquaredLength();
    
    bSnapEnabled = (sqDist <= SnapRange*SnapRange) && (sqDist > SnapDistance*SnapDistance);
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "NotifyBegin");
}

void UAnimNotifySnapToTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
    if (!bSnapEnabled)
        return;

    // Get owner actor and try to cast to Fighter
    AActor* Owner = MeshComp->GetOwner();
    if (!Owner)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimNotifyTick: No valid owner found"));
        return;
    }

    // Cast to Fighter
    AFighter* Fighter = Cast<AFighter>(Owner);
    if (!Fighter)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimNotifyTick: Owner is not a Fighter"));
        return;
    }

    // Get the lock-on target
    AActor* LockOnTarget = Fighter->GetLockOnTarget(); // Replace with your actual getter method name
    if (!LockOnTarget)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimNotifyTick: No valid lock-on target found"));
        return;
    }

    // Get current positions
    FVector CurrentLocation = Fighter->GetActorLocation();
    FVector LockOnLocation = LockOnTarget->GetActorLocation();
    FVector Diff = (LockOnLocation - CurrentLocation);
    double toTargetDistance = Diff.Length();

    // Reached the target, early out.
    if (toTargetDistance <= SnapDistance)
    {
        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Reached Target!");
        bSnapEnabled = false;
        return;
    }

    Diff /= toTargetDistance; // Normalize

    FVector DesiredLocation = (LockOnLocation - Diff*SnapDistance);
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, DesiredLocation, FrameDeltaTime, SnapSpeed);
    Fighter->SetActorLocation(NewLocation);
}

void UAnimNotifySnapToTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    bSnapEnabled = false; // Force shut off snapping.
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "NotifyEnd");
}
