// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Service/BTService_FindPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

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
		FVector OrgTargetLocation = OwnerBlackboard->GetValueAsVector(OrgTargetLocationKey.SelectedKeyName);
		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), OrgTargetLocation);
		return;
	}

	// 시야각 안에 있는지 검사 
	FVector MyForwardVector = MyCharacter->GetActorForwardVector();
	MyForwardVector.Normalize();
	ToTargetVector.Normalize();

	float DotRes = MyForwardVector.Dot(ToTargetVector);
	bool IsInSight = SightDegree > FMath::RadiansToDegrees(FMath::Acos(DotRes));
	if (!IsInSight)
	{
		FVector OrgTargetLocation = OwnerBlackboard->GetValueAsVector(OrgTargetLocationKey.SelectedKeyName);
		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), OrgTargetLocation);
		return;
	}

	// 장애물 판단
	if (!MyController->LineOfSightTo(TargetCharacter))
	{
		FVector OrgTargetLocation = OwnerBlackboard->GetValueAsVector(OrgTargetLocationKey.SelectedKeyName);
		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), OrgTargetLocation);
		return;
	}

	FVector OrgTargetLocation = OwnerBlackboard->GetValueAsVector(GetSelectedBlackboardKey());
	OwnerBlackboard->SetValueAsVector(OrgTargetLocationKey.SelectedKeyName, OrgTargetLocation);
	OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), TargetCharacter->GetActorLocation());
}
