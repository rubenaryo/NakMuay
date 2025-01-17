// Copyright Epic Games, Inc. All Rights Reserved.

#include "FightTrainingGameMode.h"
#include "CombatAction.h"

void AFightTrainingGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	// Build the combo maps
    for (const UComboSpec* pComboSpec : AvailableCombos)
    {
        if (!pComboSpec || pComboSpec->CombatActionSequence.IsEmpty())
            continue;

    	ECombatActionType lastActionInCombo = pComboSpec->CombatActionSequence.Last();
    	ECombatActionType firstActionInCombo = pComboSpec->CombatActionSequence[0];

    	// Back Combo Map
    	TArray<const UComboSpec*>* pComboArr = BackComboMap.Find(lastActionInCombo);
    	if (!pComboArr)
    	{
    		pComboArr = &BackComboMap.Emplace(lastActionInCombo);
    	}
    	pComboArr->Add(pComboSpec);

    	// Front Combo Map
    	pComboArr = FrontComboMap.Find(firstActionInCombo);
    	if (!pComboArr)
    	{
    		pComboArr = &FrontComboMap.Emplace(firstActionInCombo);
    	}
    	pComboArr->Add(pComboSpec);
    }
}

AFightTrainingGameMode::AFightTrainingGameMode()
{
}

const UAnimationAsset* AFightTrainingGameMode::GetAnimMontageForCombatActionType(ECombatActionType type) const
{
	const UAnimationAsset* const* itFind = CombatActionMontageMap.Find(type);
	return itFind ? *itFind : nullptr;
}
