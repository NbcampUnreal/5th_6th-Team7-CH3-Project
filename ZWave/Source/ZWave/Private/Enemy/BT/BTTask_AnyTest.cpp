// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/BTTask_AnyTest.h"

#include "NavigationSystem.h"

UBTTask_AnyTest::UBTTask_AnyTest()
{
	NodeName = TEXT("AnyTest");
}

EBTNodeResult::Type UBTTask_AnyTest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	NavSystem->Build();
	return EBTNodeResult::Succeeded;
}

