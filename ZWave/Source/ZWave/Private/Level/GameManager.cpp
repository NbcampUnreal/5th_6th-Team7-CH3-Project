// Copyright Epic Games, Inc. All Rights Reserved.


#include "Level/GameManager.h"
#include "Level/WaveManager.h"
#include "Base/ZWaveGameState.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Warning, TEXT("GameManagerSubsystem Initialized"));

    StartGame();
}

void UGameManager::BindPlayerEvents(APawn* PlayerPawn)
{
    if (PlayerPawn)
    {
        PlayerPawn->OnDestroyed.AddDynamic(this, &UGameManager::OnPlayerDied);
        UE_LOG(LogTemp, Log, TEXT("Player events bound to GameManager."));
    }
}

void UGameManager::StartGame()
{
    UE_LOG(LogTemp, Warning, TEXT("Game Starting..."));
    CurrentWaveNumber = 0;
    BeginPreparationPhase();
}

void UGameManager::BeginPreparationPhase()
{
    if (CurrentState == EGameState::EGS_GameOver) return;

    CurrentState = EGameState::EGS_Preparing;

    CurrentWaveNumber++;
    float PrepTime = DefaultPreparationTime;


    UE_LOG(LogTemp, Warning, TEXT("Preparation Phase Started for Wave %d. (Time: %.1fs)"), CurrentWaveNumber, PrepTime);

    if (AZWaveGameState* LocalGameState = GetGameState())
    {
        LocalGameState->SetCurrentGameState(CurrentState);
        LocalGameState->SetCurrentWave(CurrentWaveNumber);
    }

    GetWorld()->GetTimerManager().SetTimer(
        PrepPhaseTimerHandle,
        this,
        &UGameManager::BeginCombatPhase,
        PrepTime,
        false);
}

void UGameManager::BeginCombatPhase()
{
    if (CurrentState == EGameState::EGS_GameOver) return;

    GetWorld()->GetTimerManager().ClearTimer(PrepPhaseTimerHandle);
    CurrentState = EGameState::EGS_WaveInProgress;

    UE_LOG(LogTemp, Warning, TEXT("Combat Phase Started for Wave %d."), CurrentWaveNumber);

    if (AZWaveGameState* LocalGameState = GetGameState())
    {
        LocalGameState->SetCurrentGameState(CurrentState);
    }

    UWaveManager* LocalWaveManager = GetWorld()->GetSubsystem<UWaveManager>();

    if (LocalWaveManager)
    {
        LocalWaveManager->StartWave(CurrentWaveNumber);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GameManager: Cannot start combat, WaveManager is NULL!"));
    }
}

void UGameManager::OnPlayerDied(AActor* DiedActor)
{
    UE_LOG(LogTemp, Error, TEXT("Player has died."));
    EndGame(false);
}

void UGameManager::EndGame(bool bPlayerWon)
{
    if (CurrentState == EGameState::EGS_GameOver) return; 

    CurrentState = EGameState::EGS_GameOver;
    GetWorld()->GetTimerManager().ClearTimer(PrepPhaseTimerHandle);

    UE_LOG(LogTemp, Error, TEXT("GAME OVER. Player Won: %s"), bPlayerWon ? TEXT("TRUE") : TEXT("FALSE"));

    if (AZWaveGameState* LocalGameState = GetGameState())
    {
        LocalGameState->SetCurrentGameState(CurrentState);
    }
}

AZWaveGameState* UGameManager::GetGameState()
{
    if (!GameState.IsValid())
    {
        GameState = GetWorld()->GetGameState<AZWaveGameState>();
    }
    return GameState.Get();
}