// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Service/BTService_FindDest.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/PlayerTargetBaseAIController.h"
#include "Enemy/BaseEnemy.h"
#include "Level/EnemySpawnManager.h"
#include "Level/SpawnPoint.h"
#include "Base/ZWaveGameState.h"

UBTService_FindDest::UBTService_FindDest()
{
	NodeName = FString(TEXT("FindDest"));
}

void UBTService_FindDest::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	APlayerTargetBaseAIController* MyController = static_cast<APlayerTargetBaseAIController*>(OwnerComp.GetAIOwner());
	if (MyController == nullptr) return;

	AActor* SecondaryTargetActor = static_cast<AActor*>(OwnerBlackboard->GetValueAsObject(FName(TEXT("SecondaryTarget"))));
	if (SecondaryTargetActor == nullptr) return;

	if (MyController->CheckCondition(SecondaryTargetActor))
		return;

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
	if (MyCharacter == nullptr || MyCharacter->GetMaxPriorityLv() < 2) return;

	AActor* SecondaryTargetActor = static_cast<AActor*>(OwnerBlackboard->GetValueAsObject(FName(TEXT("SecondaryTarget"))));
	if (SecondaryTargetActor == nullptr) return;

	FVector NowDestLocation = OwnerBlackboard->GetValueAsVector(FName(TEXT("MoveDestTargetLocation")));

	//DrawDebugSphere(GetWorld(), MyCharacter->GetActorLocation(), SightRange, 12, FColor::Yellow, false, 0.2f);

	if (FVector::Dist2D(MyCharacter->GetActorLocation(), SecondaryTargetActor->GetActorLocation()) > SightRange)
	{
		OwnerBlackboard->ClearValue(FName(TEXT("AttackLocation")));
		OwnerBlackboard->SetValueAsBool(FName("IsAggroed"), false);
	}
	else
	{
		FVector Destination = MyController->GetAttackLocation(SecondaryTargetActor->GetActorLocation());
		OwnerBlackboard->SetValueAsVector(FName(TEXT("AttackLocation")), Destination);
	}
}

void UBTService_FindDest::TickWithIsNotAggroedCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ABaseAIController* MyController = static_cast<ABaseAIController*>(OwnerComp.GetAIOwner());
	if (MyController == nullptr) return;

	ABaseEnemy* MyCharacter = Cast<ABaseEnemy>(MyController->GetCharacter());
	if (MyCharacter == nullptr || MyCharacter->GetMaxPriorityLv() < 2) return;

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	AZWaveGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AZWaveGameState>() : nullptr;
	if (GameState == nullptr) return;

	AActor* SecondaryTargetActor = static_cast<AActor*>(OwnerBlackboard->GetValueAsObject(FName(TEXT("SecondaryTarget"))));
	if (SecondaryTargetActor == nullptr) return;

	const TArray<ASpawnPoint*>& AllSpawnPoints = GameState->GetSpawnPointArray();
	if (AllSpawnPoints.Num() == 0) return;

	if (FVector::Dist2D(MyCharacter->GetActorLocation(), SecondaryTargetActor->GetActorLocation()) < SightRange)
	{
		OwnerBlackboard->SetValueAsBool(FName("IsAggroed"), true);
		return;
	}

	FVector AttackLoc = OwnerBlackboard->GetValueAsVector(FName("AttackLocation"));
	if (!AttackLoc.IsNearlyZero())
	{
		OwnerBlackboard->ClearValue(FName(TEXT("AttackLocation")));
	}

	const FVector DestLoc = OwnerBlackboard->GetValueAsVector(FName(TEXT("MoveDestTargetLocation")));

	if (FVector::Dist2D(MyCharacter->GetActorLocation(), DestLoc) <= ReachRadius)
	{
		if (AllSpawnPoints.Num() > 1)
		{
			ASpawnPoint* NewSpawn = nullptr;
			do
			{
				NewSpawn = AllSpawnPoints[FMath::RandRange(0, AllSpawnPoints.Num() - 1)];
			} while (NewSpawn && FVector::Dist2D(NewSpawn->GetActorLocation(), DestLoc) < 10.f);

			if (NewSpawn != nullptr)
			{
				OwnerBlackboard->SetValueAsVector(FName(TEXT("MoveDestTargetLocation")), NewSpawn->GetActorLocation());
				FVector Destination = MyController->GetAttackLocation(NewSpawn->GetActorLocation());
				OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
			}
			return;
		}
	}
	else
	{
		FVector Destination = MyController->GetAttackLocation(DestLoc);
		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
	}

}
