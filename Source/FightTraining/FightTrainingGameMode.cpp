// Copyright Epic Games, Inc. All Rights Reserved.

#include "FightTrainingGameMode.h"
#include "UObject/ConstructorHelpers.h"

AFightTrainingGameMode::AFightTrainingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_PlayerFighter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
