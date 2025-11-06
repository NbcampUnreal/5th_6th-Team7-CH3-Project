// Copyright Epic Games, Inc. All Rights Reserved.

#include "Level/WaveManager.h"
#include "Level/EnemySpawnManager.h"
#include "Level/GameManager.h"
#include "Base/ZWaveGameState.h"
#include "Engine/DataTable.h"
#include "Engine/World.h"
#include "Level/ItemSpawnManager.h"


UWaveManager::UWaveManager()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> WaveConfigFinder(TEXT("DataTable'/Game/Data/DT_WaveDataTable.DT_WaveDataTable'"));

    if (WaveConfigFinder.Succeeded())
    {
        WaveDataTable = WaveConfigFinder.Object;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find DT_WaveConfig!"));
    }
}

void UWaveManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    EnemySpawnManager = GetWorld()->GetSubsystem<UEnemySpawnManager>();
    GameManager = GetWorld()->GetSubsystem<UGameManager>();
    ItemSpawnManager = GetWorld()->GetSubsystem<UItemSpawnManager>();
    UE_LOG(LogTemp, Log, TEXT("WaveManager Initialized."));

    if (EnemySpawnManager.IsValid())
    {
        EnemySpawnManager->OnEnemyDied.AddUObject(this, &UWaveManager::HandleEnemyDied);
    }
}

void UWaveManager::StartWave(int32 WaveNumber)
{
    if (!WaveDataTable || !EnemySpawnManager.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Wave Start Failed. No DataTable or SpawnManager."));
        return;
    }

    EnemiesToSpawnThisWave = 0;
    EnemiesRemainingInWave = 0;

    const FName RowName = FName(*FString::FromInt(WaveNumber));
    FWaveDataInfo* WaveData = WaveDataTable->FindRow<FWaveDataInfo>(RowName, TEXT("WaveManager"));

    CurrentWaveDataCache = WaveData;
    CurrentWaveNumber = WaveNumber;

    if (WaveData)
    {
        UE_LOG(LogTemp, Warning, TEXT("WaveData found! MonsterSpawnList contains %d entries."), WaveData->MonsterSpawnList.Num());

        for (const FMonsterSpawnInfo& SpawnInfo : WaveData->MonsterSpawnList)
        {
            EnemiesToSpawnThisWave += SpawnInfo.NumberToSpawn;

            EnemySpawnManager->RequestSpawn(SpawnInfo.MonsterClass, SpawnInfo.NumberToSpawn);

            UE_LOG(LogTemp, Warning, TEXT("Requesting spawn for Count: %d"), SpawnInfo.NumberToSpawn);
        }

        EnemiesRemainingInWave = EnemiesToSpawnThisWave;

        if (AZWaveGameState* LocalGameState = GetGameState())
        {
            LocalGameState->SetEnemiesRemaining(EnemiesRemainingInWave, EnemiesToSpawnThisWave);
        }

        UE_LOG(LogTemp, Log, TEXT("Wave %d Started. Spawning %d enemies."), WaveNumber, EnemiesToSpawnThisWave);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find Wave Data for Wave %d in DataTable!"), WaveNumber);
        OnWaveCleared();
    }

    if (WaveData)
    {
        int32 TotalMonsterCount = 0;
        for (const FMonsterSpawnInfo& SpawnInfo : WaveData->MonsterSpawnList)
        {
            TotalMonsterCount += SpawnInfo.NumberToSpawn;
        }

        if (ItemSpawnManager.IsValid())
        {
            ItemSpawnManager->GenerateDropQueue(*WaveData, TotalMonsterCount);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("WaveManager: ItemSpawnManager is NULL!"));
        }

        for (const FMonsterSpawnInfo& SpawnInfo : WaveData->MonsterSpawnList)
        {
            if (EnemySpawnManager.IsValid())
            {
                EnemySpawnManager->RequestSpawn(SpawnInfo.MonsterClass, SpawnInfo.NumberToSpawn);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("WaveManager: WavaDropData is NULL!"));
    }
}

void UWaveManager::HandleEnemyDied(ABaseCharacter* DiedEnemy)
{
    if (EnemiesRemainingInWave > 0)
    {
        EnemiesRemainingInWave--;

        if (AZWaveGameState* LocalGameState = GetGameState())
        {
            LocalGameState->SetEnemiesRemaining(EnemiesRemainingInWave, EnemiesToSpawnThisWave);
        }

        if (EnemiesRemainingInWave == 0 && EnemiesToSpawnThisWave > 0)
        {
            OnWaveCleared();
        }
    }
}

void UWaveManager::OnWaveCleared()
{
    UE_LOG(LogTemp, Warning, TEXT("Wave Cleared!"));

    EnemiesToSpawnThisWave = 0;

    if (GameManager.IsValid())
    {
        GameManager->BeginPreparationPhase();

        GameManager->GetRewardBioCoin(CurrentWaveDataCache->WaveClearRewardBioCoin);
    }

}

AZWaveGameState* UWaveManager::GetGameState()
{
    if (!GameState.IsValid())
    {
        GameState = GetWorld()->GetGameState<AZWaveGameState>();
    }
    return GameState.Get();
}
