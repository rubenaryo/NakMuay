// Fill out your copyright notice in the Description page of Project Settings.


#include "SandbagActor.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ASandbagActor::ASandbagActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add the capsule component
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	CapsuleComponent->SetupAttachment(RootComponent);

	// Create Default Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	if (Mesh)
	{
		Mesh->AlwaysLoadOnClient = true;
		Mesh->AlwaysLoadOnServer = true;
		Mesh->bOwnerNoSee = false;
		Mesh->bCastDynamicShadow = true;
		Mesh->bAffectDynamicIndirectLighting = true;
		Mesh->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		Mesh->SetupAttachment(CapsuleComponent);
		Mesh->SetCollisionProfileName(TEXT("CharacterMesh"));
		Mesh->SetGenerateOverlapEvents(true);
		Mesh->SetCanEverAffectNavigation(false);
	}
}

// Called when the game starts or when spawned
void ASandbagActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASandbagActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

