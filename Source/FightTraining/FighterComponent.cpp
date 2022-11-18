#include "FighterComponent.h"

#include "CombatAction.h"
#include "CombatComponent.h"
#include "InputAction.h"
#include "FightTrainingGameMode.h"
#include "Fighter.h"

namespace 
{
	void PrintBufferFrontToBack(const TDoubleLinkedList<FCombatAction>& buffer, int32 key, FColor color = FColor::Orange)
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
		GEngine->AddOnScreenDebugMessage(key, 1.0f, color, debugStr);
	}
}

UFighterComponent::UFighterComponent()
	: UActorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UFighterComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UFighterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (CombatActionBuffer.IsEmpty())
	{
		PrintBufferFrontToBack(ComboBuffer, 1, FColor::Orange);
		return;
	}
	
	const AController* pController = Cast<AController>(GetOwner());
	const AFighter* pFighter = nullptr;
	if (pController)
	{
		pFighter = Cast<AFighter>(pController->GetCharacter());
	}

	// FighterControllers should only ever control Fighter characters
	check(pFighter)
	if (pFighter)
	{
		// TODO: It is not sufficient to set state on the character, this causes issues with repeat combat actions, like punch->punch, which is essentially a no-op
		const FCombatAction combatAction = CombatActionBuffer.GetHead()->GetValue();
		pFighter->GetCombatComponent()->QueueCombatAction(combatAction.Type);

		// TODO: Scan Combo Buffer, if this breaks all combo sequences it needs to be cleared before this node is added to it. Right?
		
		auto* pHeadNode = CombatActionBuffer.GetHead();
		ComboBuffer.AddHead(pHeadNode->GetValue());
		CombatActionBuffer.RemoveNode(pHeadNode, true);

		ValidateComboBuffer(ComboBuffer.GetHead());
	}

	PrintBufferFrontToBack(ComboBuffer, 1, FColor::Orange);
	PrintBufferFrontToBack(CombatActionBuffer, 2, FColor::Cyan);
}

void UFighterComponent::AddToCombatActionBuffer(ECombatActionType type)
{
	static uint64_t idCounter = 0;
	FCombatAction newAction;
	newAction.SourceActionInstance = nullptr;
	newAction.ElapsedTriggeredTime = 0.0f;
	newAction.UniqueID = idCounter++;
	newAction.Type = type;
	
	CombatActionBuffer.AddTail(newAction);

	// TODO: This may have to happen on Tick() in the future,
	// the combo may be invalid if something happens when we try to consume the action. (ie: they dodge it).
	TryToAppendCombo(newAction);
}

bool UFighterComponent::TryToAppendCombo(const FCombatAction& newestAction)
{
	// TODO: Factor out to a different function
	const AFightTrainingGameMode* pGameMode = Cast<const AFightTrainingGameMode>(GetWorld()->GetAuthGameMode());

	if (!pGameMode)
		return false;

	const TArray<const UComboSpec*>* pCombosForType = pGameMode->GetCombosForLastCombatAction(newestAction.Type);

	if (!pCombosForType)
		return false;
	
	for (size_t i = 0; i != pCombosForType->Num(); ++i)
	{
		const UComboSpec* pCombo = (*pCombosForType)[i];

		check(pCombo);
		if (!pCombo)
			continue;
		
		const TArray<ECombatActionType>& comboSeq = pCombo->CombatActionSequence;
		
		auto* pComboIt = ComboBuffer.GetTail();
		bool bComboSuccess = false;

		// Scan the action buffer for a valid sequence for this combo, back-to-front
		// We don't need to scan the last element of comboSeq, we know that one's valid already (hence Num() - 2)
		for (size_t j = comboSeq.Num() - 2; j >= 0 && pComboIt; --j)
		{
			if (pComboIt->GetValue().Type != comboSeq[j])
			{
				// Failure, move onto the next combo
				break;
			}

			if (j == 0)
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
			comboAction.Type = pCombo->Type;
			CombatActionBuffer.AddTail(comboAction);
			ComboBuffer.Empty();
			return true;
		}
	}

	return false;
}

bool UFighterComponent::ValidateComboBuffer(const TDoubleLinkedList<FCombatAction>::TDoubleLinkedListNode* newlyConsumedAction)
{
	const AFightTrainingGameMode* pGameMode = Cast<const AFightTrainingGameMode>(GetWorld()->GetAuthGameMode());

	if (!pGameMode)
		return true;

	const auto* pTail = ComboBuffer.GetTail();
	const TArray<const UComboSpec*>* pCombosForType = pGameMode->GetCombosForFrontCombatAction(pTail->GetValue().Type);

	if (!pCombosForType)
	{
		// No combos, not valid.
		ComboBuffer.Empty();
		return false;
	}
	
	// Check if any combo might still be in progress.
	for (size_t i = 0; i != pCombosForType->Num(); ++i)
	{
		const UComboSpec* pCombo = (*pCombosForType)[i];

		check(pCombo);
		if (!pCombo)
		{
			// Engine error, in theory this should never be null
			continue;
		}

		if (ComboBuffer.Num() > pCombo->CombatActionSequence.Num())
		{
			// Invalid due to combo buffer being longer than allowed sequence.
			continue;
		}
		
		// We know this combo starts with the buffer's tail. Does the most recent action match?
		if (newlyConsumedAction->GetValue().Type != pCombo->CombatActionSequence[ComboBuffer.Num() - 1])
		{
			// Mismatch. Clear buffer and return false.
			ComboBuffer.Empty();
			return false;
		}
	}

	return true;
}

