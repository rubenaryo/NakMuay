// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CombatAction.h"
#include "GameFramework/GameModeBase.h"
#include "FightTrainingGameMode.generated.h"

UCLASS(minimalapi)
class AFightTrainingGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
public:
	AFightTrainingGameMode();

	const TArray<const UComboSpec*>* GetCombosForLastCombatAction(ECombatActionType combatAction) const 
	{
		return BackComboMap.Find(combatAction);
	}

	const TArray<const UComboSpec*>* GetCombosForFrontCombatAction(ECombatActionType combatAction) const
	{
		return FrontComboMap.Find(combatAction);
	}

	/**
	 * All combos available in this game mode
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<const UComboSpec*> AvailableCombos;

protected:
	/**
	 * A mapping of all combos that end with the key CombatActionType.
	 * This is used for determining what combos may have just triggered off a newly added combat action.
	 */
	TMap<ECombatActionType, TArray<const UComboSpec*>> BackComboMap;

	/**
	 * A mapping of all combos that start with the key CombatActionType.
	 * TODO: Why do we need this anymore?
	 */
	TMap<ECombatActionType, TArray<const UComboSpec*>> FrontComboMap;
};