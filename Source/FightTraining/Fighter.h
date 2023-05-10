#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fighter.generated.h"

class UCombatActorComponent;
class UShapeComponent;
class UDecalComponent;
class UPhysicalAnimationComponent;

/** 1:1 with ColliderSocketBindings **/
enum ECombatColliderArea
{
	CCA_LeftHand,
	CCA_RightHand,
	CCA_LeftKnee,
	CCA_RightKnee,
	CCA_LeftShin,
	CCA_RightShin,
	CCA_LeftFoot,
	CCA_RightFoot,
	CCA_LeftElbow,
	CCA_RightElbow,
	
	CCA_Count
};

inline FName CombatColliderSocketBindings[CCA_Count] =
{
	FName(TEXT("hand_l_Socket")),	// CCA_LeftHand
	FName(TEXT("hand_r_Socket")),	// CCA_RightHand
	FName(TEXT("knee_l_Socket")),	// CCA_LeftKnee
	FName(TEXT("knee_r_Socket")),	// CCA_RightKnee
	FName(TEXT("shin_l_Socket")),	// CCA_LeftShin
	FName(TEXT("shin_r_Socket")),	// CCA_RightShin
	FName(TEXT("foot_l_Socket")),	// CCA_LeftFoot
	FName(TEXT("foot_r_Socket")),	// CCA_RightFoot
	FName(TEXT("elbow_l_Socket")),	// CCA_LeftElbow
	FName(TEXT("elbow_r_Socket"))	// CCA_RightElbow
};

// Keep this 64bit-aligned to minimize waste from padding
// TODO
struct CombatColliderState
{
	CombatColliderState() : bEnabled(true) {};
	bool bEnabled;
};

/**
 * Fighters are characters that have combat specific behavior.
 * NOTE: This will probably not inherit from ACharacter in the future, once that class is discarded
 * in favor of something more lightweight.
**/
UCLASS(meta = (ShortTooltip = "A fighter is a type of Character that holds combat specific behavior"))
class NAKMUAY_API AFighter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatActorComponent> CombatComponent;

	UPROPERTY(EditDefaultsOnly, EditFixedSize, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TArray<UShapeComponent*> CombatColliderPrimitives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat");
	AFighter* LockOnTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDecalComponent> LockOnDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPhysicalAnimationComponent> PhysicalAnimComponent;
	
	// Sets default values for this character's properties
	AFighter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;

	// Returns whether we are locked-on
	UFUNCTION(BlueprintCallable)
	bool UpdateLockOnTarget();

	AFighter* FindNearestFighter() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintGetter, Category = "Combat")
	UCombatActorComponent* GetCombatComponent() const;

	UFUNCTION(BlueprintGetter, Category = "Combat")
	UDecalComponent* GetLockOnDecal() const;
};
