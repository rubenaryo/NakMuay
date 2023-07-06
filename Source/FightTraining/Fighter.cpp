#include "Fighter.h"

#include "FightTraining/CombatActorComponent.h"
#include "FightTraining/FighterPhysicalAnimComponent.h"
#include "Components/DecalComponent.h"
#include "Components/ShapeComponent.h"

#include "GameFramework/WorldSettings.h"

#include "EngineUtils.h"
#include "Logging/LogMacros.h"

AFighter::AFighter()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	LockOnTarget = nullptr;

	LockOnDecal = CreateDefaultSubobject<UDecalComponent>(FName(TEXT("LockOnDecal")));
	LockOnDecal->SetupAttachment(RootComponent);

	CombatComponent = CreateDefaultSubobject<UCombatActorComponent>(FName(TEXT("CombatComponent")));

	PhysicalAnimComponent = CreateDefaultSubobject<UFighterPhysicalAnimComponent>(FName(TEXT("FighterPhysicalAnimComponent")));
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

void AFighter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (!CombatComponent)
        CombatComponent = CreateDefaultSubobject<UCombatActorComponent>(FName(TEXT("FallbackCombatComponent")));

	if (!PhysicalAnimComponent)
		PhysicalAnimComponent = CreateDefaultSubobject<UFighterPhysicalAnimComponent>(FName(TEXT("FallbackFighterPhysicalAnimComp")));

	if (PhysicalAnimComponent)
		PhysicalAnimComponent->SetSkeletalMeshComponent(GetMesh());

	if (CombatComponent)
	{	
		CombatComponent->SetMovementComponent(GetCharacterMovement());
		CombatComponent->SetSkeletalMeshComponent(GetMesh());
		CombatComponent->SetPhysicalAnimComponent(GetPhysicalAnimComponent());
	}
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

UCombatActorComponent* AFighter::GetCombatComponent() const
{
	return CombatComponent;
}

UDecalComponent* AFighter::GetLockOnDecal() const
{
	return LockOnDecal;
}

UFighterPhysicalAnimComponent* AFighter::GetPhysicalAnimComponent() const
{
	return PhysicalAnimComponent;
}
