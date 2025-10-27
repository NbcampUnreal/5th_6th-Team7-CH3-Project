// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/BTTask_SetTargetLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "NavigationSystem.h"

UBTTask_SetTargetLocation::UBTTask_SetTargetLocation()
{
	NodeName = TEXT("SetTargetLocation");
}

EBTNodeResult::Type UBTTask_SetTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	NavSystem->Build();

	FNavLocation LOC;
	NavSystem->GetRandomPoint(LOC);

	//DrawDebugSphere(GetWorld(), LOC.Location, 25, 25, FColor::Red, true);
	OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), LOC.Location);

	return EBTNodeResult::Succeeded;
}
