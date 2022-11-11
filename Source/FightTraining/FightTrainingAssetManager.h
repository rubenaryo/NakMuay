// (c) rubenaryo 2022

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "FightTrainingAssetManager.generated.h"

/**
 * AssetManager override to manage game-specific assets and their initialization
 */
UCLASS()
class FIGHTTRAINING_API UFightTrainingAssetManager : public UAssetManager
{
	GENERATED_BODY()
	
public:
	UFightTrainingAssetManager() : UAssetManager() {};

	static UFightTrainingAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
