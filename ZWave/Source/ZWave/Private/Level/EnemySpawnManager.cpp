// Copyright Epic Games, Inc. All Rights Reserved.


#include "Level/EnemySpawnManager.h"
#include "Base/BaseCharacter.h"
#include "Level/SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Enemy/BaseEnemy.h"


void UEnemySpawnManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Log, TEXT("EnemySpawnManager Initialized."));
}

void UEnemySpawnManager::Deinitialize()
{
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
    }
    SpawnPoints.Empty();
    ActiveEnemies.Empty();
    Super::Deinitialize();
}

void UEnemySpawnManager::RegisterSpawnPoint(ASpawnPoint* Point)
{
    if (Point && !SpawnPoints.Contains(Point))
    {
        SpawnPoints.Add(Point);
        UE_LOG(LogTemp, Log, TEXT("SpawnPoint registered. Total: %d"), SpawnPoints.Num());
    }
}

void UEnemySpawnManager::UnregisterSpawnPoint(ASpawnPoint* Point)
{
    if (Point)
    {
        SpawnPoints.Remove(Point);
    }
}

void UEnemySpawnManager::RequestSpawn(TSubclassOf<ABaseCharacter> MonsterClass, int32 Count)
{
    if (!MonsterClass || Count <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Spawn Request REJECTED: Invalid MonsterClass or Count <= 0."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Adding %d of %s to spawn queue."), Count, *GetNameSafe(MonsterClass.Get()));

    SpawnQueue.Add(FSpawnRequest(MonsterClass, Count));

    if (GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(SpawnTimerHandle))
    {
        GetWorld()->GetTimerManager().SetTimer(
            SpawnTimerHandle,
            this,
            &UEnemySpawnManager::OnSpawnTimerTick,
            SpawnInterval,
            true);
    }
}

void UEnemySpawnManager::HandleEnemyDied(AActor* DiedActor)
{
    if (!DiedActor) return;

    ABaseEnemy* Monster = Cast<ABaseEnemy>(DiedActor);
    if (Monster)
    {
        ActiveEnemies.Remove(Monster);

        OnEnemyDied.Broadcast(Monster);
    }
}

ASpawnPoint* UEnemySpawnManager::GetRandomSpawnPoint()
{
    if (SpawnPoints.Num() == 0) return nullptr;
    int32 Index = FMath::RandRange(0, SpawnPoints.Num() - 1);
    return SpawnPoints[Index];
}

void UEnemySpawnManager::OnSpawnTimerTick()
{
    if (SpawnQueue.Num() == 0)
    {
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    int32 CurrentActiveCount = ActiveEnemies.Num();
    if (CurrentActiveCount >= MaxActiveEnemies)
    {

        return;
    }

    FSpawnRequest& CurrentRequest = SpawnQueue[0];

    if (SpawnPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("OnSpawnTimerTick: No Spawn Points available. Stopping timer."));
        GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
        return;
    }

    int32 SpawnCapacity = MaxActiveEnemies - CurrentActiveCount;

    int32 NumToSpawn = FMath::Min(SpawnsPerTick, CurrentRequest.Count);

    int32 NumToSpawnThisTick = FMath::Min(NumToSpawn, SpawnCapacity);

    for (int32 i = 0; i < NumToSpawnThisTick; ++i)
    {
        ASpawnPoint* SpawnPoint = GetRandomSpawnPoint();
        if (SpawnPoint)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            ABaseEnemy* NewMonster = GetWorld()->SpawnActor<ABaseEnemy>(
                CurrentRequest.MonsterClass,
                SpawnPoint->GetActorLocation(),
                SpawnPoint->GetActorRotation(),
                SpawnParams
            );

            if (NewMonster)
            {
                NewMonster->SpawnDefaultController();
                ActiveEnemies.Add(NewMonster);
                NewMonster->OnDestroyed.AddDynamic(this, &UEnemySpawnManager::HandleEnemyDied);
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("SpawnTick: Spawned %d. Active: %d."), NumToSpawnThisTick, ActiveEnemies.Num());

    CurrentRequest.Count -= NumToSpawnThisTick;

    if (CurrentRequest.Count <= 0)
    {
        SpawnQueue.RemoveAt(0);
    }
}