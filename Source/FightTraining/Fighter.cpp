#include "Fighter.h"

#include "FightTraining/CombatActorComponent.h"
#include "Components/DecalComponent.h"
#include "Components/ShapeComponent.h"

#include "GameFramework/WorldSettings.h"

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

	PhysicalAnimComponent = CreateDefaultSubobject<UPhysicalAnimationComponent>(FName(TEXT("PhysicalAnimComp")));
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
	const auto ToggleDecalForLockOnTarget = [this](bool bNewDecalState)
	{
		if (!LockOnTarget)
			return;

		UDecalComponent* pDecal = LockOnTarget->GetLockOnDecal();
		if (pDecal)
		{
			pDecal->SetVisibility(bNewDecalState, true);
		}
	};

	if (LockOnTarget)
	{
		ToggleDecalForLockOnTarget(false);
		LockOnTarget = nullptr;
		return false;
	}

	LockOnTarget = FindNearestFighter();
	ToggleDecalForLockOnTarget(true);
	return true;
}

AFighter* AFighter::FindNearestFighter() const
{
	static const float MAX_RADIUS_UE = 20.0f * (GetWorldSettings() ? GetWorldSettings()->WorldToMeters : 100.0f);
	static const float MAX_RADIUS_UE_SQ = MAX_RADIUS_UE * MAX_RADIUS_UE;

	AFighter* pClosestFighter = nullptr;
	float closestFighterDistanceSq = MAX_RADIUS_UE_SQ;

	for (TActorIterator<AFighter> actorItr(GetWorld()); actorItr; ++actorItr)
	{
		AFighter* pFighter = *actorItr;

		if (!pFighter || pFighter->GetUniqueID() == this->GetUniqueID())
			continue;

		FVector targetLoc = pFighter->GetActorLocation();
		FVector diff = targetLoc - this->GetActorLocation();
		float distSq = diff.SizeSquared2D();

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
