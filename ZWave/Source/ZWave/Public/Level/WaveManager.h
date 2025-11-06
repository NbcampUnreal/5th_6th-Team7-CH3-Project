// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Level/ZWaveTypes.h"
#include "WaveManager.generated.h"

class UEnemySpawnManager;
class UGameManager;
class ABaseCharacter;
class UDataTable;
class AZWaveGameState;
class UItemSpawnManager;

UCLASS()
class ZWAVE_API UWaveManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UWaveManager();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    void StartWave(int32 WaveNumber);

    const FWaveDataInfo* GetCurrentWaveData() const { return CurrentWaveDataCache; }

    int32 GetCurrentWaveNumber() const { return CurrentWaveNumber; }

    UFUNCTION(BlueprintPure, Category = "WaveManager")
    int32 GetEnemiesRemaining() const { return EnemiesRemainingInWave; }

protected:
    UFUNCTION()
    void HandleEnemyDied(ABaseCharacter* DiedEnemy);

    void OnWaveCleared();

    AZWaveGameState* GetGameState();

    UPROPERTY(EditDefaultsOnly, Category = "Wave Data")
    UDataTable* WaveDataTable;

    UPROPERTY()
    TWeakObjectPtr<UEnemySpawnManager> EnemySpawnManager;

    UPROPERTY()
    TWeakObjectPtr<UGameManager> GameManager;

    UPROPERTY()
    TWeakObjectPtr<AZWaveGameState> GameState;

    UPROPERTY()
    TWeakObjectPtr<UItemSpawnManager> ItemSpawnManager;

    FWaveDataInfo* CurrentWaveDataCache;

    UPROPERTY(Transient)
    int32 CurrentWaveNumber = 0;

    int32 EnemiesToSpawnThisWave = 0;

    int32 EnemiesRemainingInWave = 0;
};
