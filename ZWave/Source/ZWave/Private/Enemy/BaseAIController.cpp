// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseAIController.h"

#include "BrainComponent.h"


ABaseAIController::ABaseAIController()
{

}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTree == nullptr) return;

	RunBehaviorTree(BehaviorTree);
}

void ABaseAIController::StopBehaviorTree()
{
	UBrainComponent* BrainComp = BrainComponent.Get();
	if (BrainComp)
	{
		BrainComp->StopLogic(FString(TEXT("Death")));
	}
}
