#include "Fighter.h"

#include "FightTraining/CombatActorComponent.h"
#include "Components/DecalComponent.h"
#include "Components/ShapeComponent.h"

#include "EngineUtils.h"

AFighter::AFighter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CombatColliderPrimitives.Init(nullptr, CCA_Count);
	//for (uint32_t i = 0; i != ECombatColliderArea::CCA_Count; ++i)
	//{
	//	//if (CombatColliderPrimitives[i])
	//		CombatColliderPrimitives[i]->SetupAttachment(GetMesh(), CombatColliderSocketBindings[i]);
	//}

	LockOnTarget = nullptr;
	LockOnDecal = CreateDefaultSubobject<UDecalComponent>(FName(TEXT("LockOnDecal")));
	LockOnDecal->SetupAttachment(RootComponent);
	
}

void AFighter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	static const FAttachmentTransformRules skAttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	
	for (uint32_t i = 0; i != ECombatColliderArea::CCA_Count; ++i)
	{
		UShapeComponent* pPrim = CombatColliderPrimitives[i];

		if (pPrim)
		{
			if (!pPrim->AttachToComponent(GetMesh(), skAttachmentRules, CombatColliderSocketBindings[i]))
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, CombatColliderSocketBindings[i].GetPlainNameString());			
			}
			
			pPrim->SetHiddenInGame(false);
			pPrim->SetVisibility(true);
		}
	}

	if (CombatComponent)
		CombatComponent->SetSkeletalMeshComponent(GetMesh());
}

bool AFighter::UpdateLockOnTarget()
{
	FString debugStr;

	const auto ToggleDecalForLockOnTarget = [this](bool bNewDecalState)
	{
		if (!LockOnTarget)
			return;

		UDecalComponent* pDecal = LockOnTarget->GetLockOnDecal();
		if (pDecal)
		{
			pDecal->SetActive(bNewDecalState, true);
			pDecal->SetVisibility(bNewDecalState, true);
		}
	};

	if (LockOnTarget)
	{
		ToggleDecalForLockOnTarget(false);
		LockOnTarget = nullptr;
		debugStr.Appendf(TEXT("Null"));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, debugStr);
		return false;
	}

	LockOnTarget = FindNearestFighter();
	ToggleDecalForLockOnTarget(true);
	debugStr.Appendf(TEXT("0x%x"), LockOnTarget ? LockOnTarget->GetUniqueID() : 0);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, debugStr);
	return true;
}

AFighter* AFighter::FindNearestFighter() const
{
	static const float MAX_RADIUS_TO_SELF_SQ = 1000.0f;

	AFighter* pClosestFighter = nullptr;
	float closestFighterDistanceSq = FLT_MAX;

	for (TActorIterator<AFighter> actorItr(GetWorld()); actorItr; ++actorItr)
	{
		AFighter* pFighter = *actorItr;

		if (!pFighter || pFighter->GetUniqueID() == this->GetUniqueID())
			continue;

		float distSq = ((pFighter->GetActorLocation() - this->GetActorLocation()).SizeSquared());
		if (distSq < closestFighterDistanceSq)
		{
			pClosestFighter = pFighter;
			closestFighterDistanceSq = distSq;
		}
	}

	return pClosestFighter;
}

// Called when the game starts or when spawned
void AFighter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFighter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UCombatActorComponent* AFighter::GetCombatComponent() const
{
	return CombatComponent;
}

UDecalComponent* AFighter::GetLockOnDecal() const
{
	return LockOnDecal;
}
