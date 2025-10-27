// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseAIController.h"


ABaseAIController::ABaseAIController()
{

}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeComp == nullptr) return;

	RunBehaviorTree(BehaviorTreeComp);
}
