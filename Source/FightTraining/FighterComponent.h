#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatAction.h"
#include "FighterComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIGHTTRAINING_API UFighterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFighterComponent();
	//UFighterComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void AddToCombatActionBuffer(ECombatActionType type);

private:

	// See if the newest action is the final action of any combos. If so, add the combo as an action to the combat action buffer.
	bool TryToAppendCombo(const FCombatAction& newestAction);

	// If valid, return true. If invalid, empty the buffer and return false
	bool ValidateComboBuffer(const TDoubleLinkedList<FCombatAction>::TDoubleLinkedListNode* newlyConsumedAction);
	
	// Head is the oldest action, Tail is the newest
	TDoubleLinkedList<FCombatAction> CombatActionBuffer;

	// All consumed actions go into this intermediate combo buffer, which is used to validate combo actions.
	TDoubleLinkedList<FCombatAction> ComboBuffer;
};