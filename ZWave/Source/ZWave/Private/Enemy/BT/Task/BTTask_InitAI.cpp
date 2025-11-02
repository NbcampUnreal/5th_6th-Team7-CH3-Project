// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Task/BTTask_InitAI.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

#include "Base/ZWaveGameMode.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/BaseAIController.h"
#include "Prop/PurificationDevice.h"

UBTTask_InitAI::UBTTask_InitAI()
{
	NodeName = TEXT("InitAI");
}


EBTNodeResult::Type UBTTask_InitAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return EBTNodeResult::Failed;

	ABaseAIController* AIController = static_cast<ABaseAIController*>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) return EBTNodeResult::Failed;

	ABaseEnemy* MyCharacter = static_cast<ABaseEnemy*>(AIController->GetCharacter());
	if (MyCharacter == nullptr) return EBTNodeResult::Failed;

	if (OwnerBlackboard->IsVectorValueSet(GetSelectedBlackboardKey()) == false)
	{
		AZWaveGameMode* GameMode = static_cast<AZWaveGameMode*>(UGameplayStatics::GetGameMode(this));
		if (GameMode == nullptr) return EBTNodeResult::Failed;
		if (GameMode->PuriDevice == nullptr) return EBTNodeResult::Failed;

		FVector TargetLocation = GameMode->PuriDevice->GetActorLocation();

		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (!NavSystem) return EBTNodeResult::Failed;

		FVector ToTarget = TargetLocation - MyCharacter->GetActorLocation();
		ToTarget.Normalize();

		FNavLocation ProjectedLocation;
		bool bFoundNavLocation = NavSystem->ProjectPointToNavigation(TargetLocation - ToTarget * MyCharacter->GetAttackRange(), ProjectedLocation, FVector(100, 100, 100.0f));
		FVector Destination = bFoundNavLocation ? ProjectedLocation.Location : TargetLocation - ToTarget * MyCharacter->GetAttackRange();

		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);

		//return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}
