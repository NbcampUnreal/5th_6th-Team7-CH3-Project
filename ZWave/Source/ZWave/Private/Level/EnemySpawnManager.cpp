// Copyright Epic Games, Inc. All Rights Reserved.


#include "Level/EnemySpawnManager.h"
#include "Base/BaseCharacter.h"
#include "Level/SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


void UEnemySpawnManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Log, TEXT("EnemySpawnManager Initialized."));
}

void UEnemySpawnManager::Deinitialize()
{
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
    if (!MonsterClass || SpawnPoints.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Spawn Request Failed. No Spawn Points or Invalid Class."));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Spawning %d of %s"), Count, *MonsterClass->GetName());

    for (int32 Index = 0; Index < Count; ++Index)
    {
        ASpawnPoint* SpawnPoint = GetRandomSpawnPoint();
        if (SpawnPoint)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            ABaseCharacter* NewMonster = GetWorld()->SpawnActor<ABaseCharacter>(
                MonsterClass,
                SpawnPoint->GetActorLocation(),
                SpawnPoint->GetActorRotation(),
                SpawnParams
            );

            if (NewMonster)
            {
                ActiveEnemies.Add(NewMonster);
                NewMonster->SpawnDefaultController();
                NewMonster->OnDestroyed.AddDynamic(this, &UEnemySpawnManager::HandleEnemyDied);
            }
        }
    }
}

void UEnemySpawnManager::HandleEnemyDied(AActor* DiedActor)
{
    if (!DiedActor) return;

    ABaseCharacter* Monster = Cast<ABaseCharacter>(DiedActor);
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