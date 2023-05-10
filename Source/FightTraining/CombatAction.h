// (c) rubenaryo 2022

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "CombatAction.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "CombatActionType"))
enum class ECombatActionType : uint8
{
	None			UMETA(DisplayName="None"),
	Lead_Straight	UMETA(DisplayName="Lead Straight"),
	Rear_Straight	UMETA(DisplayName="Rear Straight"),
	Rear_Kick		UMETA(DisplayName="Rear Kick"),
	Combo_Test		UMETA(DisplayName="DEBUG: Combo Test"),
	Count
};

const FString& GetCombatActionNameFromType(ECombatActionType type);

USTRUCT(BlueprintType)
struct FCombatAction
{
	GENERATED_BODY()
	
	const FInputActionInstance* SourceActionInstance;
	float ElapsedTriggeredTime = 0.0f;
	uint64_t UniqueID = 0;
	ECombatActionType Type = ECombatActionType::None;
	bool bWasInterrupted = false;
	
	float GetTriggeredTime() const { return ElapsedTriggeredTime; }
};

UCLASS(BlueprintType)
class NAKMUAY_API UComboSpec : public UDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * Defines the necessary combat actions needed to trigger this combo
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<ECombatActionType> CombatActionSequence;

	/**
	 * On Combo success, consumes the CombatActionSequence and buffers a CombatAction of this type.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ECombatActionType Type = ECombatActionType::None;
};