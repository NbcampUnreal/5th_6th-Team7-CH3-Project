// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseAIController.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

#include "Enemy/BaseEnemy.h"


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

FVector ABaseAIController::GetAttackLocation(FVector TargetLocation)
{
	ABaseEnemy* MyCharacter = Cast<ABaseEnemy>(GetCharacter());
	if (MyCharacter == nullptr)  FVector().ZeroVector;

	FVector ToTarget = (TargetLocation - MyCharacter->GetActorLocation()).GetSafeNormal();
	float AttakRange = MyCharacter->GetAttackRange();

	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavSystem) return FVector().ZeroVector;

	FNavLocation ProjectedLocation;
	bool bFoundNavLocation = NavSystem->ProjectPointToNavigation(TargetLocation - ToTarget * AttakRange, ProjectedLocation, FVector(100, 100, 100.0f));
	FVector Destination = bFoundNavLocation ? ProjectedLocation.Location : TargetLocation - ToTarget * AttakRange;

	return Destination;
}

void ABaseAIController::SetValueAsVectorToBlackboard(FName BlackboardKeyName, FVector Value)
{
	UBlackboardComponent* OwnerBlackboardComp = GetBlackboardComponent();
	if (OwnerBlackboardComp == nullptr) return;

	OwnerBlackboardComp->SetValueAsVector(BlackboardKeyName, Value);
}

void ABaseAIController::SetValueAsBoolToBlackboard(FName BlackboardKeyName, bool Value)
{
	UBlackboardComponent* OwnerBlackboardComp = GetBlackboardComponent();
	if (OwnerBlackboardComp == nullptr) return;

	OwnerBlackboardComp->SetValueAsBool(BlackboardKeyName, Value);
}

void ABaseAIController::SetValueAsObjectToBlackboard(FName BlackboardKeyName, UObject* Object)
{
	UBlackboardComponent* OwnerBlackboardComp = GetBlackboardComponent();
	if (OwnerBlackboardComp == nullptr) return;

	OwnerBlackboardComp->SetValueAsObject(BlackboardKeyName, Object);
}

