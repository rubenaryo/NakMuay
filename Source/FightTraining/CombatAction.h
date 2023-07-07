// (c) rubenaryo 2022

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "CombatAction.generated.h"

UENUM(BlueprintType, meta = (ScriptName = "CombatActionType"))
enum class ECombatActionType : uint8
{
    None			UMETA(DisplayName = "None"),
    Lead_Straight	UMETA(DisplayName = "Lead Straight"),
    Rear_Straight	UMETA(DisplayName = "Rear Straight"),
    Lead_Kick		UMETA(DisplayName = "Lead Kick"),
    Rear_Kick		UMETA(DisplayName = "Rear Kick"),
    Lead_LowKick	UMETA(DisplayName = "Lead Low Kick"),
    Rear_LowKick	UMETA(DisplayName = "Rear Low Kick"),
    Lead_Teep		UMETA(DisplayName = "Lead Teep"),
    Rear_Teep		UMETA(DisplayName = "Rear Teep"),
    Lead_Hook		UMETA(DisplayName = "Lead Hook"),
    Rear_Hook		UMETA(DisplayName = "Rear Hook"),
	Lead_Uppercut	UMETA(DisplayName = "Lead Uppercut"),
    Rear_Uppercut	UMETA(DisplayName = "Rear Uppercut"),
    Lead_Knee		UMETA(DisplayName = "Lead Knee"),
    Rear_Knee		UMETA(DisplayName = "Rear Knee"),
    Lead_Elbow		UMETA(DisplayName = "Lead Elbow"),
    Rear_Elbow		UMETA(DisplayName = "Rear Elbow"),
    Combo_Test		UMETA(DisplayName = "DEBUG: Combo Test"),
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

UCLASS(BlueprintType)
class NAKMUAY_API UCombatActionToHitProfile : public UDataAsset
{
	GENERATED_BODY()

	UCombatActionToHitProfile();

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> CombatActionToHitProfile;
};