// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Service/BTService_FindPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

#include "Base/BaseCharacter.h"
#include "Enemy/BaseEnemy.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	NodeName = FString(TEXT("FindPlayer"));
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* MyController = OwnerComp.GetAIOwner();
	if (MyController == nullptr) return;

	ABaseEnemy* MyCharacter = Cast<ABaseEnemy>(MyController->GetCharacter());
	if (MyCharacter == nullptr || MyCharacter->GetCanEditAttackPriority() == false) return;

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (TargetCharacter == nullptr) return;

	// 시야범위 안에 있는지 검사
	FVector ToTargetVector = TargetCharacter->GetActorLocation() - MyCharacter->GetActorLocation();
	if (ToTargetVector.Length() > SightRange)
	{
		OwnerBlackboard->ClearValue(GetSelectedBlackboardKey());
		return;
	}

	// 시야각 안에 있는지 검사 
	FVector MyForwardVector = MyCharacter->GetActorForwardVector().GetSafeNormal();
	float DotRes = MyForwardVector.Dot(ToTargetVector.GetSafeNormal());
	bool IsInSight = SightDegree > FMath::RadiansToDegrees(FMath::Acos(DotRes));
	if (!IsInSight)
	{
		// When the player instantly moves behind an enemy and goes out of its field of view, 
		// the enemy immediately loses sight of the player
		if (!(ToTargetVector.Size() < AutoDetectionRange && OwnerBlackboard->IsVectorValueSet(GetSelectedBlackboardKey())))
		{
			UE_LOG(LogTemp, Display, TEXT("%f < %f !!!!!!!"), ToTargetVector.Size(), AutoDetectionRange);
			OwnerBlackboard->ClearValue(GetSelectedBlackboardKey());
			return;
		}
	}

	// 장애물 판단
	if (!MyController->LineOfSightTo(TargetCharacter))
	{
		OwnerBlackboard->ClearValue(GetSelectedBlackboardKey());
		return;
	}

	// 플레이어와의 거리가 공격범위 이내이다 -> 정지
	if (ToTargetVector.Size() < MyCharacter->GetAttackRange())
	{
		UE_LOG(LogTemp, Display, TEXT("Player is In My Attack Range -> Stop Moving"));
		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), MyCharacter->GetActorLocation());
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Player Is out of my Attack Range -> Move to Attack Position"));
		// 그렇지 않다 -> 접근

		FVector TargetLocation = TargetCharacter->GetActorLocation();
		FVector ToTarget = (TargetLocation - MyCharacter->GetActorLocation()).GetSafeNormal();
		float AttakRange = MyCharacter->GetAttackRange();

		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (!NavSystem) return;

		FNavLocation ProjectedLocation;
		bool bFoundNavLocation = NavSystem->ProjectPointToNavigation(TargetLocation - ToTarget * AttakRange, ProjectedLocation, FVector(100, 100, 100.0f));
		FVector Destination = bFoundNavLocation ? ProjectedLocation.Location : TargetLocation - ToTarget * AttakRange;

		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
	}

	//OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), TargetCharacter->GetActorLocation());
}
