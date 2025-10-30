// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAIController();

public:
	virtual void OnPossess(APawn* InPawn) override;


/// <summary>
/// BT 관련함수
/// </summary>
public:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree = nullptr;

public:
	void StopBehaviorTree();

public:
	FVector GetAttackLocation(FVector TargetLocation);
	void SetValueAsVectorToBlackboard(FName BlackboardKeyName, FVector Value);
	void SetValueAsBoolToBlackboard(FName BlackboardKeyName, bool Value);
	void SetValueAsObjectToBlackboard(FName BlackboardKeyName, UObject* Object);
};
