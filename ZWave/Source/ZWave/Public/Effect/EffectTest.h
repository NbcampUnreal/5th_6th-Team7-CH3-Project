// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EffectTest.generated.h"


UCLASS()
class ZWAVE_API AEffectTest : public AActor
{
	GENERATED_BODY()
	
public:	
	AEffectTest();

	FORCEINLINE float GetSpeed() const 
	{
		return Speed; 
	}

	FORCEINLINE void SetSpeed(float Value)
	{
		Speed = Value;
	}

	void Move();
	void TestEffect();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEST")
	USceneComponent* Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEST")
	UStaticMeshComponent* SM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEST")
	class UModeDefinition* ModeData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TEST")
	float Speed;

	FTimerHandle TestHandle;
	FTimerHandle EffectHandle;
};
