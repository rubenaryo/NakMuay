#include "Fighter.h"

#include "FightTraining/CombatComponent.h"
#include "Components/ShapeComponent.h"

// Sets default values
AFighter::AFighter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	
	CombatColliderPrimitives.Init(dynamic_cast<UShapeComponent*>(UShapeComponent::StaticClass()->ClassDefaultObject), CCA_Count);
	for (uint32_t i = 0; i != ECombatColliderArea::CCA_Count; ++i)
	{
		if (CombatColliderPrimitives[i])
			CombatColliderPrimitives[i]->SetupAttachment(GetMesh(), CombatColliderSocketBindings[i]);
	}
}

void AFighter::PreRegisterAllComponents()
{
	Super::PreRegisterAllComponents();
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

// Called to bind functionality to input
void AFighter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFighter::PunchAction()
{
	for (uint32_t i = 0; i != ECombatColliderArea::CCA_Count; ++i)
	{
		UShapeComponent* pColliderPrimitive = CombatColliderPrimitives[i];

		if (pColliderPrimitive)
		{
			pColliderPrimitive->ShapeColor = FColor::Green;
			pColliderPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			pColliderPrimitive->SetGenerateOverlapEvents(true);
			pColliderPrimitive->MarkRenderStateDirty();
		}
	}
	CombatComponent->Punch();
}

void AFighter::ResetCombat()
{
	for (uint32_t i = 0; i != ECombatColliderArea::CCA_Count; ++i)
	{
		UShapeComponent* pColliderPrimitive = CombatColliderPrimitives[i];

		if (pColliderPrimitive)
		{
			pColliderPrimitive->ShapeColor = FColor::Red;
			pColliderPrimitive->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			pColliderPrimitive->SetGenerateOverlapEvents(false);
			pColliderPrimitive->MarkRenderStateDirty();
		}
	}
	CombatComponent->ResetCombatState();
}