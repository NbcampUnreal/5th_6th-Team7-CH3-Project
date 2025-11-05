// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/PlayerTargetBaseAIController.h"
#include "ClimbingAIController.generated.h"

class AClimbingPoint;
/**
 * 
 */
UCLASS()
class ZWAVE_API AClimbingAIController : public APlayerTargetBaseAIController
{
	GENERATED_BODY()
	
public:
	AClimbingAIController();

	virtual bool CheckCondition(AActor* Target) override;

	void ResetClimbingPoint();

	void StartClimbing();
	void StopClimbing();

protected:

	AClimbingPoint* TargetClimbingPoint;
};
