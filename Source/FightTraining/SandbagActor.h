// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SandbagActor.generated.h"

class UCapsuleComponent;

UCLASS()
class FIGHTTRAINING_API ASandbagActor : public AActor
{
	GENERATED_BODY()

private:
	/** The CapsuleComponent being used for movement collision **/
	UPROPERTY(Category = Collision, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	/** The main skeletal mesh associated with this Sandbag Actor **/
	UPROPERTY(Category = Actor, VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;
	
public:	
	// Sets default values for this actor's properties
	ASandbagActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Returns Mesh subobject **/
	FORCEINLINE const UStaticMeshComponent* GetMesh() const { return Mesh; }

	/** Returns CapsuleComponent subobject **/
	FORCEINLINE const UCapsuleComponent* GetCapsuleComponent() const { return CapsuleComponent; }
};
