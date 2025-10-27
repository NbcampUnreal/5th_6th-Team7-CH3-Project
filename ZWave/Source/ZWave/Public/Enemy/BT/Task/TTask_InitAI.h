// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "TTask_InitAI.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UTTask_InitAI : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UTTask_InitAI();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
