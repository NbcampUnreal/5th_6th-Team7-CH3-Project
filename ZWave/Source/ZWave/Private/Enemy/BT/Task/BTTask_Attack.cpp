// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Task/BTTask_Attack.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Enemy/BaseAIController.h"
#include "Enemy/BaseEnemy.h"

UBTTask_Attack::UBTTask_Attack()
{
	NodeName = FString(TEXT("Attack"));
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseAIController* AIController = static_cast<ABaseAIController*>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) return EBTNodeResult::Failed;

	ABaseEnemy* AICharacter = static_cast<ABaseEnemy*>(AIController->GetCharacter());
	if (AICharacter == nullptr) return EBTNodeResult::Failed;

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	if (OwnerBlackboard->GetValueAsBool(FName(TEXT("IsTargetToFake"))) == false)
	{
		AICharacter->Attack();
	}

	return EBTNodeResult::Succeeded;
}