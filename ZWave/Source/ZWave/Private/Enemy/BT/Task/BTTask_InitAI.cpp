// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Task/BTTask_InitAI.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

#include "Base/ZWaveGameMode.h"
#include "Prop/PurificationDevice.h"

UBTTask_InitAI::UBTTask_InitAI()
{
	NodeName = TEXT("InitAI");
}


EBTNodeResult::Type UBTTask_InitAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return EBTNodeResult::Failed;

	if (OwnerBlackboard->IsVectorValueSet(GetSelectedBlackboardKey()) == false)
	{
		AZWaveGameMode* GameMode = static_cast<AZWaveGameMode*>(UGameplayStatics::GetGameMode(this));
		if (GameMode == nullptr) return EBTNodeResult::Failed;
		if (GameMode->PuriDevice == nullptr) return EBTNodeResult::Failed;

		FVector TargetLocation = GameMode->PuriDevice->GetActorLocation();

		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (!NavSystem) return EBTNodeResult::Failed;

		// 위치를 네비게이션 메시에 프로젝션
		FNavLocation ProjectedLocation;
		bool bFoundNavLocation = NavSystem->ProjectPointToNavigation(TargetLocation, ProjectedLocation, FVector(100.0f, 100.0f, 100.0f));
		FVector Destination = bFoundNavLocation ? ProjectedLocation.Location : TargetLocation;

		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
	}

	return EBTNodeResult::Succeeded;
}
