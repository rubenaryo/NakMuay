// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatAction.h"

static const FString CA_TYPE_NAMES[] =
{
	TEXT("None"),
	TEXT("Lead Straight"),
	TEXT("Rear Straight"),
	TEXT("Rear Kick"),
	TEXT("Combo_Test")
};

const FString& GetCombatActionNameFromType(ECombatActionType type)
{
	return CA_TYPE_NAMES[(uint8)type];
}

UCombatActionToHitProfile::UCombatActionToHitProfile() : UDataAsset()
{
	CombatActionToHitProfile.SetNum((int32_t)ECombatActionType::Count, false);

	for (uint8_t i = 0; i != (uint8_t)ECombatActionType::Count; ++i)
	{
		CombatActionToHitProfile[i] = FName(GetCombatActionNameFromType((ECombatActionType)i));
	}
}