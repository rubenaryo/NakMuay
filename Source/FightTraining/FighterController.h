#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CombatAction.h"
#include "FighterController.generated.h"

class UInputAction;
struct FInputActionInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FIGHTTRAINING_API AFighterController : public APlayerController
{
	GENERATED_BODY()

public:	
	AFighterController();

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void TickActor( float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction ) override;

	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void AddToInputBuffer(const UInputAction* CompletedInputAction, ECombatActionType type);

private:

	// See if the newest action is the final action of any combos. If so, add the combo as an action to the combat action buffer.
	bool TryToAppendCombo(const FCombatAction& newestAction);

	// If valid, return true. If invalid, empty the buffer and return false
	bool ValidateComboBuffer(const TDoubleLinkedList<FCombatAction>::TDoubleLinkedListNode& newlyConsumedAction);
	
	// Head is the oldest action, Tail is the newest
	TDoubleLinkedList<FCombatAction> CombatActionBuffer;

	// All consumed actions go into this intermediate combo buffer, which is used to validate combo actions.
	TDoubleLinkedList<FCombatAction> ComboBuffer;
	bool bComboBufferDirty;
};
