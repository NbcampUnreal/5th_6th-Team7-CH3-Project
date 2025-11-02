// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Task/BTTask_InitPlayerTargetAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseAIController.h"
#include "Enemy/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

UBTTask_InitPlayerTargetAI::UBTTask_InitPlayerTargetAI()
{
	NodeName = TEXT("InitPlayerTargetAI");
}


EBTNodeResult::Type UBTTask_InitPlayerTargetAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return EBTNodeResult::Failed;

	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) return EBTNodeResult::Failed;

	ABaseEnemy* MyCharacter = Cast<ABaseEnemy>(AIController->GetCharacter());
	if (MyCharacter == nullptr) return EBTNodeResult::Failed;

	if (OwnerBlackboard->IsVectorValueSet(GetSelectedBlackboardKey()) == false)
	{
		ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(MyCharacter, 0);
		if (PlayerPawn == nullptr) return EBTNodeResult::Failed;

		OwnerBlackboard->SetValueAsObject(FName(TEXT("SecondaryTarget")), PlayerPawn);
		OwnerBlackboard->SetValueAsVector(FName(TEXT("SecondaryTargetLocation")), PlayerPawn->GetActorLocation());

		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (!NavSystem) return EBTNodeResult::Failed;

		FVector TargetLocation = PlayerPawn->GetActorLocation();
		FVector ToTarget = (TargetLocation - MyCharacter->GetActorLocation()).GetSafeNormal();

		FNavLocation ProjectedLocation;
		const float AttackRange = MyCharacter->GetAttackRange();

		bool bFoundNavLocation = NavSystem->ProjectPointToNavigation(
			TargetLocation - ToTarget * AttackRange,
			ProjectedLocation,
			FVector(100, 100, 100.0f)
		);

		const FVector Destination = bFoundNavLocation
			? ProjectedLocation.Location
			: TargetLocation - ToTarget * AttackRange;

		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
	}

	OwnerBlackboard->SetValueAsBool(FName(TEXT("IsAggroed")), false);

	return EBTNodeResult::Succeeded;
}