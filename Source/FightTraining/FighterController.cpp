#include "FighterController.h"

#include "CombatAction.h"
#include "CombatComponent.h"
#include "InputAction.h"
#include "EnhancedPlayerInput.h"
#include "FightTrainingGameMode.h"
#include "Fighter.h"

namespace 
{
	void PrintBufferFrontToBack(const TDoubleLinkedList<FCombatAction>& buffer)
	{
		if (!GEngine)
			return;

		const auto* pHead = buffer.GetHead();
		FString debugStr;
		for (int i = 0; i != buffer.Num(); ++i)
		{
			const FCombatAction combatAction = pHead->GetValue();
			debugStr.Appendf(TEXT("%s, "), ToCStr(GetCombatActionNameFromType(combatAction.Type)));
			pHead = pHead->GetNextNode();
		}
		GEngine->AddOnScreenDebugMessage(1,  1.0f, FColor::Orange, debugStr);
	}
}

AFighterController::AFighterController() :
	bComboBufferDirty(false)
{
}

void AFighterController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFighterController::BeginPlay()
{
	Super::BeginPlay();
}

void AFighterController::TickActor( float DeltaSeconds, ELevelTick TickType, FActorTickFunction& ThisTickFunction )
{
	Super::TickActor(DeltaSeconds, TickType, ThisTickFunction);
	
	if (CombatActionBuffer.IsEmpty())
	{
		PrintBufferFrontToBack(ComboBuffer);
		return;
	}
	
	AFighter* pFighter = Cast<AFighter>(GetCharacter());

	// FighterControllers should only ever control Fighter characters
	check(pFighter)
	if (pFighter && pFighter->GetCombatComponent()->GetCurrentCombatActionType() == ECombatActionType::None)
	{
		const FCombatAction combatAction = CombatActionBuffer.GetHead()->GetValue();
		pFighter->GetCombatComponent()->SetCurrentCombatActionType(combatAction.Type);

		// TODO: Scan Combo Buffer, if this breaks all combo sequences it needs to be cleared before this node is added to it. Right?
		
		auto* pHeadNode = CombatActionBuffer.GetHead();
		ComboBuffer.AddHead(pHeadNode->GetValue());
		CombatActionBuffer.RemoveNode(pHeadNode, true);

		bool bComboBufferValid = ValidateComboBuffer(*ComboBuffer.GetHead());

		if (!bComboBufferValid)
			int stub = 42;
		
		//GEngine->AddOnScreenDebugMessage(-1,  3.0f, FColor::Orange, FString::Printf(TEXT("Consumed %ls"), ToCStr(GetCombatActionNameFromType(combatAction.Type))));
	}

	// TODO: This should be done per-combat action
	static constexpr uint64_t FRAME_TOLERANCE = 60 * 2;
	static uint64_t sFrameAccumulator = 0;

	// Dequeue the oldest input after FRAME_TOLERANCE frames
	if (sFrameAccumulator++ >= FRAME_TOLERANCE)
	{
		//InputActionBuffer.RemoveNode(InputActionBuffer.GetHead(), true);
		sFrameAccumulator = 0;
	}

	PrintBufferFrontToBack(ComboBuffer);
}

void AFighterController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AFighterController::AddToInputBuffer(const UInputAction* CompletedInputAction, ECombatActionType type)
{
	const UEnhancedPlayerInput* pPlayerInput = Cast<const UEnhancedPlayerInput>(this->PlayerInput);
	check(pPlayerInput);

	const FInputActionInstance* pInputActionInstance = NULL;
	if (pPlayerInput)
	{
		pInputActionInstance = pPlayerInput->FindActionInstanceData(CompletedInputAction);
	}

	if (pInputActionInstance)
	{
		static uint64_t idCounter = 0;
		FCombatAction newAction;
		newAction.SourceActionInstance = pInputActionInstance;
		newAction.ElapsedTriggeredTime = pInputActionInstance->GetTriggeredTime();
		newAction.UniqueID = idCounter++;
		newAction.Type = type;
		
		CombatActionBuffer.AddTail(newAction);
		
		TryToAppendCombo(newAction);
	}
}

bool AFighterController::TryToAppendCombo(const FCombatAction& newestAction)
{
	// TODO: Factor out to a different function
	const AFightTrainingGameMode* pGameMode = Cast<const AFightTrainingGameMode>(GetWorld()->GetAuthGameMode());

	if (!pGameMode)
		return false;

	const TArray<const UComboSpec*>* pCombosForType = pGameMode->GetCombosForLastCombatAction(newestAction.Type);

	if (!pCombosForType)
		return false;

	// TODO: Iterate all combos not just the first one
	const UComboSpec* pCombo0 = (*pCombosForType)[0];

	if (pCombo0)
	{
		const TArray<ECombatActionType>& comboSeq = pCombo0->CombatActionSequence;
			
		auto* pComboIt = ComboBuffer.GetTail();
		bool bComboSuccess = false;

		// Scan the action buffer for a valid sequence for this combo, back-to-front
		// We don't need to scan the last element of comboSeq, we know that one's valid already (hence Num() - 2)
		for (int i = comboSeq.Num() - 2; i >= 0 && pComboIt; --i)
		{
			if (pComboIt->GetValue().Type != comboSeq[i])
			{
				// Failure, move onto the next Combo
				break;
			}

			if (i == 0)
			{
				// Success, all combo inputs have been scanned and none exited early.
				bComboSuccess = true;
				break;
			}
				
			pComboIt = pComboIt->GetPrevNode();
		}

		// Consume all combo CombatActions and replace with one for the combo itself.
		if (bComboSuccess)
		{
			FCombatAction comboAction;
			comboAction.SourceActionInstance = newestAction.SourceActionInstance;
			comboAction.ElapsedTriggeredTime = newestAction.ElapsedTriggeredTime;
			comboAction.UniqueID = newestAction.UniqueID + 1;
			comboAction.Type = pCombo0->Type;
			CombatActionBuffer.AddTail(comboAction);
			ComboBuffer.Empty();
			return true;
		}
	}

	return false;
}

bool AFighterController::ValidateComboBuffer(const TDoubleLinkedList<FCombatAction>::TDoubleLinkedListNode& newlyConsumedAction)
{
	const AFightTrainingGameMode* pGameMode = Cast<const AFightTrainingGameMode>(GetWorld()->GetAuthGameMode());

	if (!pGameMode)
		return true;

	auto* pTail = ComboBuffer.GetTail();
	const TArray<const UComboSpec*>* pCombosForType = pGameMode->GetCombosForFrontCombatAction(pTail->GetValue().Type);

	if (pCombosForType)
	{
		// Check if any combo might still be in progress.
		for (size_t i = 0; i != pCombosForType->Num(); ++i)
		{
			const UComboSpec* pCombo = (*pCombosForType)[i];
			
			// We know this combo starts with the buffer's tail. Does the most recent action match?
			if (newlyConsumedAction.GetValue().Type != pCombo->CombatActionSequence[ComboBuffer.Num() - 1])
			{
				// Mismatch. Clear buffer and return false.
				ComboBuffer.Empty();
				return false;
			}
		}
	}
	else
	{
		// No combos, not valid.
		ComboBuffer.Empty();
		return false;
	}
	
	return true;
}

