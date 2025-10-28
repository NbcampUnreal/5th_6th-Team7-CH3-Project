// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/ZWaveGameState.h"
#include "UI/IngameHUD.h"


void AZWaveGameState::SetCurrentGameState(EGameState NewState)
{
    CurrentGameState = NewState;
}

void AZWaveGameState::SetCurrentWave(int32 NewWave)
{
    CurrentWave = NewWave;

    if (IngameHUD)
    {

        IngameHUD->OnWaveChanged(CurrentWave, 20);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("GameState: SetCurrentWave called, but LocalIngameHUD is NULL."));
    }
}

void AZWaveGameState::SetEnemiesRemaining(int32 NewCount, int32 MaxCount)
{

    if (IngameHUD)
    {
        IngameHUD->OnEnemyCountChanged(NewCount, MaxCount);
    }
}

void AZWaveGameState::RegisterIngameHUD(UIngameHUD* InHUD)
{
    IngameHUD = InHUD;
}