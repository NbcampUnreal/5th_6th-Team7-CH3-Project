// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "ClimbingEnemy.generated.h"

UENUM(BlueprintType)
enum class EClimbingState : uint8
{
	None,
	Wall,
	Ceiling
};

/**
 * 
 */
UCLASS()
class ZWAVE_API AClimbingEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	AClimbingEnemy();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Die() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EClimbingState GetClimbingState() const { return State; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|ClimbingSpeed")
	float ClimbingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|ClimbingSpeed")
	float EndCeiling;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat|ClimbingSpeed")
	EClimbingState State;

	UFUNCTION()
	void OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TWeakObjectPtr<ACharacter> Target;

	FRotator DesiredMeshRot = FRotator::ZeroRotator;

};
