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

	MyController->CheckCondition(SecondaryTargetActor);

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

	AActor* SecondaryTargetActor = static_cast<AActor*>(OwnerBlackboard->GetValueAsObject(FName(TEXT("SecondaryTarget"))));
	if (SecondaryTargetActor == nullptr) return;

	AZWaveGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AZWaveGameState>() : nullptr;
	if (GameState == nullptr) return;

	const float MySightRange = MyCharacter->GetSightRange();
	const bool CheckAggroed = FVector::Dist2D(MyCharacter->GetActorLocation(), SecondaryTargetActor->GetActorLocation()) <= MySightRange;

	if (!CheckAggroed)
	{
		OwnerBlackboard->SetValueAsBool(FName("IsAggroed"), false);

		const TArray<ASpawnPoint*>& AllSpawnPoints = GameState->GetSpawnPointArray();
		if (AllSpawnPoints.Num() == 0) return;

		if (AllSpawnPoints.Num() > 1)
		{
			ASpawnPoint* NewSpawn = nullptr;
			do
			{
				NewSpawn = AllSpawnPoints[FMath::RandRange(0, AllSpawnPoints.Num() - 1)];
			} while (NewSpawn && FVector::Dist2D(NewSpawn->GetActorLocation(), MyCharacter->GetActorLocation()) < 10.f);

			const FVector NewDest = NewSpawn->GetActorLocation();
			OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), NewDest);
			return;
		}
	}
	else
	{
		FVector Destination = MyController->GetAttackLocation(SecondaryTargetActor->GetActorLocation());
		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), Destination);
	}
}

void UBTService_FindDest::TickWithIsNotAggroedCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ABaseAIController* MyController = static_cast<ABaseAIController*>(OwnerComp.GetAIOwner());
	if (MyController == nullptr) return;

	ABaseEnemy* MyCharacter = Cast<ABaseEnemy>(MyController->GetCharacter());
	if (MyCharacter == nullptr || MyCharacter->GetCanEditAttackPriority() == false) return;

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return;

	AZWaveGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AZWaveGameState>() : nullptr;
	if (GameState == nullptr) return;

	AActor* SecondaryTargetActor = static_cast<AActor*>(OwnerBlackboard->GetValueAsObject(FName(TEXT("SecondaryTarget"))));
	if (SecondaryTargetActor == nullptr) return;

	const TArray<ASpawnPoint*>& AllSpawnPoints = GameState->GetSpawnPointArray();
	if (AllSpawnPoints.Num() == 0) return;

	const float MySightRange = MyCharacter->GetSightRange();
	const bool IsAggroed = FVector::Dist2D(MyCharacter->GetActorLocation(), SecondaryTargetActor->GetActorLocation()) <= MySightRange;

	//DrawDebugSphere(GetWorld(), MyCharacter->GetActorLocation(), MySightRange, 12, FColor::Yellow, false, 0.2f);

	if (IsAggroed)
	{
		OwnerBlackboard->SetValueAsBool(FName("IsAggroed"), true);
		return;
	}

	const FVector SelfLoc = MyCharacter->GetActorLocation();
	const FVector DestLoc = OwnerBlackboard->GetValueAsVector(GetSelectedBlackboardKey());

	const bool bReachedDest = FVector::Dist2D(SelfLoc, DestLoc) <= ReachRadius;
	if (bReachedDest)
	{
		if (AllSpawnPoints.Num() > 1)
		{
			ASpawnPoint* NewSpawn = nullptr;
			do
			{
				NewSpawn = AllSpawnPoints[FMath::RandRange(0, AllSpawnPoints.Num() - 1)];
			} while (NewSpawn && FVector::Dist2D(NewSpawn->GetActorLocation(), DestLoc) < 10.f);

			const FVector NewDest = NewSpawn->GetActorLocation();
			OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), NewDest);
			return;
		}
	}

}
