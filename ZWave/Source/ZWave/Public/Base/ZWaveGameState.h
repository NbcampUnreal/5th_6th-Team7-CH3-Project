// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Level/ZWaveTypes.h"
#include "ZWaveGameState.generated.h"

UCLASS()
class ZWAVE_API AZWaveGameState : public AGameStateBase
{
    GENERATED_BODY()

public:

    void SetCurrentGameState(EGameState NewState)
    {
        CurrentGameState = NewState;
    }

    void SetCurrentWave(int32 NewWave)
    {
        CurrentWave = NewWave;
    }

    void SetEnemiesRemaining(int32 NewCount)
    {
        EnemiesRemaining = NewCount;
    }

    UFUNCTION(BlueprintPure, Category = "Game State")
    EGameState GetCurrentGameState() const { return CurrentGameState; }

    UFUNCTION(BlueprintPure, Category = "Game State")
    int32 GetCurrentWave() const { return CurrentWave; }

    UFUNCTION(BlueprintPure, Category = "Game State")
    int32 GetEnemiesRemaining() const { return EnemiesRemaining; }

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    EGameState CurrentGameState = EGameState::EGS_None;

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    int32 CurrentWave = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Game State")
    int32 EnemiesRemaining = 0;
};