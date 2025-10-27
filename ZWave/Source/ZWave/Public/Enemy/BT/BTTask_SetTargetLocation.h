// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_SetTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UBTTask_SetTargetLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:
	UBTTask_SetTargetLocation();

protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
