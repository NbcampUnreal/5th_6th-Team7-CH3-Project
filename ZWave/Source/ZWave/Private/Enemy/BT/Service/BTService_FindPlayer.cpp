// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Service/BTService_FindPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"

#include "Base/BaseCharacter.h"
#include "Enemy/BaseAIController.h"
#include "Enemy/BaseEnemy.h"

UBTService_FindPlayer::UBTService_FindPlayer()
{
	NodeName = FString(TEXT("FindPlayer"));
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	ABaseAIController* MyController = static_cast<ABaseAIController*>(OwnerComp.GetAIOwner());
	if (MyController == nullptr) return;

	ABaseEnemy* MyCharacter = Cast<ABaseEnemy>(MyController->GetCharacter());
	if (MyCharacter == nullptr || MyCharacter->GetMaxPriorityLv() < 2) return;

	bool bIsAggroed = OwnerBlackboard->GetValueAsBool(FName(TEXT("IsAggroed")));
	if (bIsAggroed)
	{
		TickWithIsAggroedCondtion(OwnerComp, NodeMemory, DeltaSeconds, MyController, MyCharacter);
	}
	else
	{
		TickWithIsNotAggroedCondition(OwnerComp, NodeMemory, DeltaSeconds, MyController, MyCharacter);
	}
}

void UBTService_FindPlayer::TickWithIsAggroedCondtion(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds, ABaseAIController* MyController, ABaseEnemy* MyCharacter)
{
	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (TargetCharacter == nullptr) return;

	FVector ToTargetVector = TargetCharacter->GetActorLocation() - MyCharacter->GetActorLocation();

	// 어그로에 끌리면 해당 타겟을 일단 인식범위 안에 한번은 꼭넣어야함 
	// -> 이후 어그로 해제(타겟은 시야범위안에있는 타겟이 됨, 이후 놓치면 놓치는거임)
	if (ToTargetVector.Size() < SightRange)
	{
		OwnerBlackboard->SetValueAsBool(FName("IsAggroed"), false);
	}
	else
	{
		AActor* SecondaryTargetActor = static_cast<AActor*>(OwnerBlackboard->GetValueAsObject(FName(TEXT("SecondaryTarget"))));
		FVector Destination = MyController->GetAttackLocation(SecondaryTargetActor->GetActorLocation());

		//OwnerBlackboard->SetValueAsVector(FName(TEXT("SecondaryTargetLocation")), SecondaryTargetActor->GetActorLocation());
		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
	}
}

void UBTService_FindPlayer::TickWithIsNotAggroedCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds, ABaseAIController* MyController, ABaseEnemy* MyCharacter)
{
	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (TargetCharacter == nullptr) return;

	FVector ToTargetVector = TargetCharacter->GetActorLocation() - MyCharacter->GetActorLocation();

	// 시야범위 안에 들어왔는지
	if (CheckSightRange(ToTargetVector, SightRange) == false)
	{
		ClearTarget(OwnerBlackboard);
		return;
	}

	// 시야각 안에 들어왔는지
	bool bIsSecondaryTargetLocation = OwnerBlackboard->IsVectorValueSet(GetSelectedBlackboardKey());
	if (CheckSightDegree(MyCharacter, ToTargetVector, bIsSecondaryTargetLocation) == false)
	{
		ClearTarget(OwnerBlackboard);
		return;
	}

	// 장애물은 없는지
	if (!MyController->LineOfSightTo(TargetCharacter))
	{
		ClearTarget(OwnerBlackboard);
		return;
	}

	FVector TargetLocation = TargetCharacter->GetActorLocation();
	OwnerBlackboard->SetValueAsObject(FName(TEXT("SecondaryTarget")), TargetCharacter);
	if (ToTargetVector.Size() < MyCharacter->GetAttackRange()) // 공격범위 안에있다면 -> 정지
	{
		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), MyCharacter->GetActorLocation());
	}
	else  // 공격범위 밖에 있다면 -> 이동
	{
		FVector Destination = MyController->GetAttackLocation(TargetCharacter->GetActorLocation());
		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
	}
}

bool UBTService_FindPlayer::CheckSightRange(FVector ToTargetVector, float sightRange)
{
	return !(ToTargetVector.Length() > SightRange);
}

bool UBTService_FindPlayer::CheckSightDegree(AActor* StdActor, FVector ToTargetVector, bool bIsSecondaryTargetLocationSet)
{
	FVector MyForwardVector = StdActor->GetActorForwardVector().GetSafeNormal();
	float DotRes = MyForwardVector.Dot(ToTargetVector.GetSafeNormal());
	bool IsInSight = SightDegree > FMath::RadiansToDegrees(FMath::Acos(DotRes));
	if (!IsInSight)
	{
		// When the player instantly moves behind an enemy and goes out of its field of view, 
		// the enemy immediately loses sight of the player
		if (!(ToTargetVector.Size() < AutoDetectionRange && bIsSecondaryTargetLocationSet))
		{
			return false;
		}
	}
	return true;
}

void UBTService_FindPlayer::ClearTarget(UBlackboardComponent* OwnerBlackboard)
{
	OwnerBlackboard->ClearValue(FName(TEXT("SecondaryTarget")));
	OwnerBlackboard->ClearValue(GetSelectedBlackboardKey());
}
