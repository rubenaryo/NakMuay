#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fighter.generated.h"

class UCombatActorComponent;
class UShapeComponent;
class UDecalComponent;
class UFighterPhysicalAnimComponent;

/**
 * Fighters are characters that have combat specific behavior.
 * NOTE: This will probably not inherit from ACharacter in the future, once that class is discarded
 * in favor of something more lightweight.
**/
UCLASS(meta = (ShortTooltip = "A fighter is a type of Character that holds combat specific behavior"))
class NAKMUAY_API AFighter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFighter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCombatActorComponent> CombatComponent; // Initialized in BP Construction Script

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat");
	AFighter* LockOnTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDecalComponent> LockOnDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UFighterPhysicalAnimComponent> PhysicalAnimComponent; // Initialized in BP Construction Script

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void PostInitializeComponents() override;

	// Returns whether we are locked-on
	UFUNCTION(BlueprintCallable)
	bool UpdateLockOnTarget();

	AFighter* FindNearestFighter() const;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintGetter, Category = "Combat")
	UCombatActorComponent* GetCombatComponent() const;

	UFUNCTION(BlueprintGetter, Category = "Combat")
	UDecalComponent* GetLockOnDecal() const;

    UFUNCTION(BlueprintGetter, Category = "Combat")
    UFighterPhysicalAnimComponent* GetPhysicalAnimComponent() const;
};
