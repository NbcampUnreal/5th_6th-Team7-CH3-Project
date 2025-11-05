// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Level/ZWaveTypes.h"
#include "ZWaveGameState.generated.h"

class UIngameHUD;
class ASpawnPoint;
class AClimbingPoint;

UCLASS()
class ZWAVE_API AZWaveGameState : public AGameStateBase
{
    GENERATED_BODY()

public:

    UFUNCTION(BlueprintPure, Category = "Game State")
    EGameState GetCurrentGameState() const { return CurrentGameState; }

    UFUNCTION(BlueprintPure, Category = "Game State")
    int32 GetCurrentWave() const { return CurrentWave; }

    UFUNCTION(BlueprintPure, Category = "Game State")
    int32 GetEnemiesRemaining() const { return EnemiesRemaining; }

    UFUNCTION(BlueprintPure, Category = "Game State")
    TArray<ASpawnPoint*> GetSpawnPointArray() { return SpawnPoints; }

    UFUNCTION(BlueprintPure, Category = "Game State")
    TArray<AClimbingPoint*> GetClimbingPointArray() { return ClimbingPoints; }

    void SetCurrentGameState(EGameState NewState);

    void SetCurrentWave(int32 NewWave);

    void SetEnemiesRemaining(int32 NewCount, int32 MaxCount);

    void RegisterIngameHUD(UIngameHUD* InHUD);

    void RegisterSpawnPoint(ASpawnPoint* SP);

    void RegisterClimbingPoint(AClimbingPoint* CP);

    UIngameHUD* GetIngameHUD();

    UIngameHUD* IngameHUD;

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    EGameState CurrentGameState = EGameState::EGS_None;

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    int32 CurrentWave = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    int32 EnemiesRemaining = 0;

    TArray<ASpawnPoint*> SpawnPoints;
    TArray<AClimbingPoint*> ClimbingPoints;
};