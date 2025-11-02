// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Task/BTTask_InitPlayerTargetAI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseAIController.h"
#include "Enemy/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Level/SpawnPoint.h"
#include "Base/ZWaveGameState.h"

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

	AZWaveGameState* GameState = GetWorld() ? GetWorld()->GetGameState<AZWaveGameState>() : nullptr;
	if (GameState == nullptr) return EBTNodeResult::Failed;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(MyCharacter, 0);
	if (Player == nullptr) return EBTNodeResult::Failed;

	const TArray<ASpawnPoint*>& AllSpawnPoints = GameState->GetSpawnPointArray();
	if (AllSpawnPoints.Num() == 0) return EBTNodeResult::Failed;

	OwnerBlackboard->SetValueAsObject(GetSelectedBlackboardKey(), Player);

	if (AllSpawnPoints.Num() > 1)
	{
		ASpawnPoint* NewSpawn = nullptr;
		do
		{
			NewSpawn = AllSpawnPoints[FMath::RandRange(0, AllSpawnPoints.Num() - 1)];
		} while (NewSpawn && FVector::Dist2D(NewSpawn->GetActorLocation(), MyCharacter->GetActorLocation()) < 10.f);

		const FVector NewDest = NewSpawn->GetActorLocation();
		OwnerBlackboard->SetValueAsVector(FName("MoveDestLocation"), NewDest);
	}

	return EBTNodeResult::Succeeded;
}