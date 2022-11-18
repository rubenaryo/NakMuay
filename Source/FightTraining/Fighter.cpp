#include "Fighter.h"

#include "FightTraining/CombatComponent.h"
#include "Components/ShapeComponent.h"

AFighter::AFighter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CombatColliderPrimitives.Init(nullptr, CCA_Count);
	//for (uint32_t i = 0; i != ECombatColliderArea::CCA_Count; ++i)
	//{
	//	//if (CombatColliderPrimitives[i])
	//		CombatColliderPrimitives[i]->SetupAttachment(GetMesh(), CombatColliderSocketBindings[i]);
	//}
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

UCombatComponent* AFighter::GetCombatComponent() const
{
	return CombatComponent;
}