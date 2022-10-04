#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fighter.generated.h"

class UCombatComponent;
class USphereComponent;

/**
 * Fighters are characters that have combat specific behavior.
 * NOTE: This will probably not inherit from ACharacter in the future, once that class is discarded
 * in favor of something more lightweight.
 */
UCLASS(meta = (ShortTooltip = "A fighter is a type of Character that holds combat specific behavior"))
class FIGHTTRAINING_API AFighter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USphereComponent* LeftFistCollider;

public:
	// Sets default values for this character's properties
	AFighter();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void PunchAction();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void ResetCombat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
