// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Task/BTTask_RotateToTarget.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/BaseAIController.h"

UBTTask_RotateToTarget::UBTTask_RotateToTarget()
{
	NodeName = FString(TEXT("RotateToTarget"));
}

EBTNodeResult::Type UBTTask_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return EBTNodeResult::Failed;

	ABaseAIController* AIController = static_cast<ABaseAIController*>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) return EBTNodeResult::Failed;

	ABaseEnemy* MyCharacter = static_cast<ABaseEnemy*>(AIController->GetCharacter());
	if (MyCharacter == nullptr) return EBTNodeResult::Failed;

	FVector TargetLocation = OwnerBlackboard->GetValueAsVector(GetSelectedBlackboardKey());
	FVector ToTargetVector = (TargetLocation - MyCharacter->GetActorLocation()).GetSafeNormal();

	FRotator LookAtRot = ToTargetVector.Rotation();
	FRotator NewRot = FRotator(0, LookAtRot.Yaw, 0);

	MyCharacter->SetActorRotation(NewRot);

	return EBTNodeResult::Succeeded;
}