// (c) rubenaryo 2022

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FighterMovementComponent.generated.h"

/**
 * Custom Character Movement Component for Fighters.
 */
UCLASS()
class NAKMUAY_API UFighterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetOverrideMaxSpeed(float overrideMaxSpeed) { OverrideMaxSpeed = overrideMaxSpeed; }

	UFUNCTION(BlueprintCallable)
	void ResetOverrideMaxSpeed() { OverrideMaxSpeed = -1.0f; }
	
	// UMovementComponent Interface
	virtual float GetMaxSpeed() const override;

protected:
	float OverrideMaxSpeed = -1.0f;
};
