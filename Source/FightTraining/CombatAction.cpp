// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatAction.h"

static const FString CA_TYPE_NAMES[ECombatActionType::Count] =
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

