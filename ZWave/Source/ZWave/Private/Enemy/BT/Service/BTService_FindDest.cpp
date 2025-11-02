// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Service/BTService_FindDest.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseAIController.h"
#include "Enemy/RangedEnemy.h"
#include "Level/EnemySpawnManager.h"
#include "Level/SpawnPoint.h"

UBTService_FindDest::UBTService_FindDest()
{
	NodeName = FString(TEXT("FindDest"));
}

void UBTService_FindDest::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	bool bIsAggroed = OwnerBlackboard->GetValueAsBool(FName(TEXT("IsAggroed")));
	if (bIsAggroed)
	{
		TickWithIsAggroedCondtion(OwnerComp, NodeMemory, DeltaSeconds);
	}
	else
	{
		TickWithIsNotAggroedCondition(OwnerComp, NodeMemory, DeltaSeconds);
	}
}

void UBTService_FindDest::TickWithIsAggroedCondtion(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	ABaseAIController* MyController = static_cast<ABaseAIController*>(OwnerComp.GetAIOwner());
	if (MyController == nullptr) return;

	ABaseEnemy* MyCharacter = Cast<ABaseEnemy>(MyController->GetCharacter());
	if (MyCharacter == nullptr || MyCharacter->GetCanEditAttackPriority() == false) return;

	//여긴 나중에 포탑 찾는걸로 바뀌는거죠?
	ABaseCharacter* TargetCharacter = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (TargetCharacter == nullptr) return;

	FVector ToTargetVector = TargetCharacter->GetActorLocation() - MyCharacter->GetActorLocation();

	if (ToTargetVector.Size() < SightRange)
	{
		OwnerBlackboard->SetValueAsBool(FName("IsAggroed"), false);
	}
	else
	{
		AActor* SecondaryTargetActor = static_cast<AActor*>(OwnerBlackboard->GetValueAsObject(FName(TEXT("SecondaryTarget"))));
		FVector Destination = MyController->GetAttackLocation(SecondaryTargetActor->GetActorLocation());

		OwnerBlackboard->SetValueAsVector(FName(TEXT("SecondaryTargetLocation")), SecondaryTargetActor->GetActorLocation());
		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
	}
}

void UBTService_FindDest::TickWithIsNotAggroedCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//여기서 해야할일
	/*
	당장의 이동 목표(다른 스폰 위치) 체크
	이동 목표에 도달했다면 플레이어 위치 참조
	플레이어 위치가 일정 거리 내라면 계단 이동하여 공격 가능한 위치까지 접근(플레이어가 2층에 있다면)
	(스폰 위치 == 계단이 있는 위치)
	2층

	     복도
	 복도중앙복도 
		 복도

	복도 끝 아래가 스폰위치 이기에 플레이어가 2층에 있다고 가정하면 본인이 스폰된 복도 위쪽에 있을경우 올라간다고 생각하면 될듯
	몬스터는 공격 가능한 위치까지 계속해서 접근(일정 거리에 왔다고 멈추는게 아니라 공격이 가능해진 시점에 멈춘다)
	*/
	ABaseAIController* MyController = static_cast<ABaseAIController*>(OwnerComp.GetAIOwner());
	if (MyController == nullptr) return;

	ARangedEnemy* MyCharacter = Cast<ARangedEnemy>(MyController->GetCharacter());
	if (MyCharacter == nullptr || MyCharacter->GetCanEditAttackPriority() == false) return;

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	UEnemySpawnManager* SpawnManager = GetWorld()->GetSubsystem<UEnemySpawnManager>();
	if (!SpawnManager) return;

	const TArray<ASpawnPoint*>& AllSpawnPoints = SpawnManager->GetSpawnPoints();
	if (AllSpawnPoints.Num() == 0) return;

	const FVector SelfLoc = MyCharacter->GetActorLocation();
	const FVector DestLoc = OwnerBlackboard->GetValueAsVector(TEXT("MoveDestLocation"));

	ABaseCharacter* MainTarget = Cast<ABaseCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!MainTarget) return;

	AActor* TargetActor = Cast<AActor>(OwnerBlackboard->GetValueAsObject(FName(TEXT("SecondaryTarget"))));
	if (TargetActor == nullptr)
		OwnerBlackboard->SetValueAsObject(TEXT("SecondaryTarget"), MainTarget);

	const bool bReachedDest = FVector::Dist2D(SelfLoc, DestLoc) <= ReachRadius;
	if (bReachedDest)
	{
		const FVector TargetLoc = MainTarget->GetActorLocation();
		const float DistToTarget = FVector::Dist(SelfLoc, TargetLoc);

		if (DistToTarget <= SightRange)
		{
			OwnerBlackboard->SetValueAsObject(TEXT("SecondaryTarget"), MainTarget);
			OwnerBlackboard->SetValueAsVector(TEXT("SecondaryTargetLocation"), TargetLoc);
			OwnerBlackboard->SetValueAsBool(TEXT("IsAggroed"), true);
			return;
		}
		else
		{
			if (AllSpawnPoints.Num() > 1)
			{
				ASpawnPoint* NewSpawn = nullptr;
				do
				{
					NewSpawn = AllSpawnPoints[FMath::RandRange(0, AllSpawnPoints.Num() - 1)];
				} while (NewSpawn && FVector::Dist2D(NewSpawn->GetActorLocation(), SelfLoc) < 10.f);

				const FVector NewDest = NewSpawn->GetActorLocation();
				OwnerBlackboard->SetValueAsVector(TEXT("MoveDestLocation"), NewDest);
			}
		}
	}   
}
