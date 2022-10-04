#include "Fighter.h"

#include "FightTraining/CombatComponent.h"
#include "Components/SphereComponent.h" // For colliders

// Sets default values
AFighter::AFighter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	LeftFistCollider = CreateDefaultSubobject<USphereComponent>(TEXT("LeftFistCollider"));
	LeftFistCollider->SetupAttachment(GetMesh());
}

static const FName LeftFistSocketName = FName(TEXT("hand_l_socket"));

// Called when the game starts or when spawned
void AFighter::BeginPlay()
{
	Super::BeginPlay();

	static const FAttachmentTransformRules skAttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);

	if (!GetMesh()->DoesSocketExist(LeftFistSocketName) || !LeftFistCollider->AttachToComponent(GetMesh(), skAttachmentRules, LeftFistSocketName) && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, LeftFistSocketName.GetPlainNameString());
	}
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
	LeftFistCollider->ShapeColor = FColor::Green;
	LeftFistCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftFistCollider->SetGenerateOverlapEvents(true);
	LeftFistCollider->MarkRenderStateDirty();
	CombatComponent->Punch();
}

void AFighter::ResetCombat()
{
	LeftFistCollider->ShapeColor = FColor::Red;
	LeftFistCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	LeftFistCollider->SetGenerateOverlapEvents(false);
	LeftFistCollider->MarkRenderStateDirty();
	CombatComponent->ResetCombatState();
}