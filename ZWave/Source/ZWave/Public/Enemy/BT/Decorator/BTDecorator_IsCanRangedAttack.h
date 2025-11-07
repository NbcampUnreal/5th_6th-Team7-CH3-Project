// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsCanRangedAttack.generated.h"

class UBlackboardComponent;
/**
 * 
 */
UCLASS()
class ZWAVE_API UBTDecorator_IsCanRangedAttack : public UBTDecorator
{
	GENERATED_BODY()
public:
	UBTDecorator_IsCanRangedAttack();
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
