// Copyright Epic Games, Inc. All Rights Reserved.


#include "Level/GameManager.h"
#include "Level/WaveManager.h"
#include "Base/ZWaveGameState.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "UI/IngameHUD.h"
#include "Kismet/GameplayStatics.h"         
#include "EngineUtils.h"                   
#include "Level/DoorControlPanel.h"         
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h" 
#include "GameFramework/Actor.h"
#include "Effect/SpeedBuffEffect.h"
#include "Effect/EffectApplyManager.h"
#include "Effect/EffectBase.h"


void UGameManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Warning, TEXT("GameManagerSubsystem Initialized"));

    GetWorld()->GetTimerManager().SetTimer(
        GameStartTimerHandle,
        this,
        &UGameManager::StartGame,
        1.0f,
        false
    );
    EffectManager = GetWorld()->GetSubsystem<UEffectApplyManager>();
    StimEffectClass = USpeedBuffEffect::StaticClass();

    AllManagedPointLights.Empty();
    AllManagedSpotLights.Empty();
    AllControlPanelsInLevel.Empty();

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

    UWorld* World = GetWorld();
    if (World)
    {
        TArray<AActor*> FoundLightActors;
        UGameplayStatics::GetAllActorsWithTag(World, FName("ManagedLight"), FoundLightActors);
        for (AActor* Actor : FoundLightActors)
        {
            UPointLightComponent* LightComp = Actor->FindComponentByClass<UPointLightComponent>();
            if (LightComp)
            {
                AllManagedPointLights.Add(LightComp);
            }

            USpotLightComponent* SpotLightComp = Actor->FindComponentByClass<USpotLightComponent>();
            if (SpotLightComp)
            {
                AllManagedSpotLights.Add(SpotLightComp);
            }
        }

        for (TActorIterator<ADoorControlPanel> It(World); It; ++It)
        {
            AllControlPanelsInLevel.Add(*It);
        }

        UE_LOG(LogTemp, Log, TEXT("GameManager: Found %d PointLights, %d SpotLights, %d ControlPanels."),
            AllManagedPointLights.Num(), AllManagedSpotLights.Num(), AllControlPanelsInLevel.Num());
    }
}

void UGameManager::BeginPreparationPhase()
{
    if (CurrentState == EGameState::EGS_GameOver) return;

    CurrentState = EGameState::EGS_Preparing;

    CurrentWaveNumber++;

    UE_LOG(LogTemp, Warning, TEXT("Preparation Phase Started for Wave %d. (Time: 30s)"), CurrentWaveNumber);

    if (AZWaveGameState* LocalGameState = GetGameState())
    {
        LocalGameState->SetCurrentGameState(CurrentState);
        LocalGameState->SetCurrentWave(CurrentWaveNumber);

        UIngameHUD* LocalHUD = LocalGameState->GetIngameHUD();
        if (IsValid(LocalHUD))
        {
            LocalHUD->OnChangedWaveMode(false);
            LocalHUD->OnBreakTimeStart();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GameManger : IngameHUD was Invalid."), CurrentWaveNumber);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(
        PrepPhaseTimerHandle,
        this,
        &UGameManager::BeginCombatPhase,
        DefaultPreparationTime,
        false);

    if (bIsStimUsedThisWave)
    {
        bIsStimUsedThisWave = false;
        OnStimStateChanged.Broadcast(false);
        UE_LOG(LogTemp, Log, TEXT("GameManager: Stims Recharged."));
    }
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
        LocalGameState->SetCurrentWave(CurrentWaveNumber);

        UIngameHUD* LocalHUD = LocalGameState->GetIngameHUD();
        if (IsValid(LocalHUD))
        {
            LocalHUD->OnChangedWaveMode(true);
            LocalHUD->OnBattleStart();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("GameManger : IngameHUD was Invalid."), CurrentWaveNumber);
        }
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

    bBlackoutUsedThisWave = false;
    if (FMath::FRand() < BlackoutChancePerWave)
    {
        StartBlackoutEvent();
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
    GetWorld()->GetTimerManager().ClearTimer(BlackoutTimerHandle);

    UE_LOG(LogTemp, Error, TEXT("GAME OVER. Player Won: %s"), bPlayerWon ? TEXT("TRUE") : TEXT("FALSE"));

    if (AZWaveGameState* LocalGameState = GetGameState())
    {
        LocalGameState->SetCurrentGameState(CurrentState);
    }

    if (bIsBlackoutActive)
    {
        EndBlackoutEvent();
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

void UGameManager::StartBlackoutEvent()
{
    if (bIsBlackoutActive || bBlackoutUsedThisWave) return;

    bIsBlackoutActive = true;
    bBlackoutUsedThisWave = true;
    UE_LOG(LogTemp, Warning, TEXT("--- BLACKOUT EVENT STARTED ---"));

    OnBlackoutStateChanged.Broadcast(true);

    for (UPointLightComponent* LightComp : AllManagedPointLights)
    {
        if (LightComp)
        {
            LightComp->SetVisibility(false);
        }
    }

    for (USpotLightComponent* LightComp : AllManagedSpotLights)
    {
        if (LightComp)
        {
            LightComp->SetVisibility(false);
        }
    }

    for (ADoorControlPanel* Panel : AllControlPanelsInLevel)
    {
        if (Panel)
        {
            Panel->UnlockAllDoors();
        }
    }

    GetWorld()->GetTimerManager().SetTimer(
        BlackoutTimerHandle,
        this,
        &UGameManager::EndBlackoutEvent,
        BlackoutDuration,
        false);
}

void UGameManager::EndBlackoutEvent()
{
    bIsBlackoutActive = false;
    GetWorld()->GetTimerManager().ClearTimer(BlackoutTimerHandle);
    UE_LOG(LogTemp, Warning, TEXT("--- BLACKOUT EVENT ENDED ---"));

    OnBlackoutStateChanged.Broadcast(false);

    for (UPointLightComponent* LightComp : AllManagedPointLights)
    {
        if (LightComp)
        {
            LightComp->SetVisibility(true);
        }
    }

    for (USpotLightComponent* LightComp : AllManagedSpotLights)
    {
        if (LightComp)
        {
            LightComp->SetVisibility(true);
        }
    }
}

void UGameManager::UseStim(AActor* Interactor)
{
    if (!StimEffectClass)
    {
        UE_LOG(LogTemp, Error, TEXT("GameManager::UseStim - StimEffectClass is NULL!"));
        return;
    }

    if (bIsStimUsedThisWave)
    {
        UE_LOG(LogTemp, Log, TEXT("Stim already used this wave."));
        return;
    }

    if (!Interactor) return;

    bIsStimUsedThisWave = true;

    OnStimStateChanged.Broadcast(true);
    UE_LOG(LogTemp, Log, TEXT("GameManager: Stim Used!"));

    TArray<TSubclassOf<UEffectBase>> EffectArray;
    EffectArray.Add(StimEffectClass);
    EffectManager->ApplyEffect(Interactor, EffectArray, StimBuffDuration);
}