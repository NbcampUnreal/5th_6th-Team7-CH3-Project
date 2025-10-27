// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_AnyTest.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UBTTask_AnyTest : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_AnyTest();


protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
