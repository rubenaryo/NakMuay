// (c) rubenaryo 2022

#include "FightTrainingAssetManager.h"

UFightTrainingAssetManager& UFightTrainingAssetManager::Get()
{
	check(GEngine);

	UFightTrainingAssetManager* pAssetManager = Cast<UFightTrainingAssetManager>(GEngine->AssetManager);
	return *pAssetManager;
}

void UFightTrainingAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
}
