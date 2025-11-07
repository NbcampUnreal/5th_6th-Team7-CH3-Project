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
#include "Prop/Turret.h"
#include "Item/InventoryComponent.h"
#include "Level/EnemySpawnManager.h"
#include "Player/TaskPlayer.h"

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

    EnemySpawnManager = GetWorld()->GetSubsystem<UEnemySpawnManager>();

    if (EnemySpawnManager.IsValid())
    {
        EnemySpawnManager->OnEnemyDied.AddUObject(this, &UGameManager::HandleEnemyKilled);
    }
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
    CumulativeKills = 0;
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

        TArray<AActor*> FoundTurretSpawns;
        UGameplayStatics::GetAllActorsWithTag(World, FName("TurretSpawnPoint"), FoundTurretSpawns);

        for (AActor* SpawnActor : FoundTurretSpawns)
        {
            if (SpawnActor)
            {
                TurretSpawnTransforms.Add(SpawnActor->GetActorTransform());
            }
        }
    }

    {
        FSoftClassPath ItemClassPath(TEXT("Blueprint'/Game/Prop/Turret/BP_Turret.BP_Turret_C'"));
        UClass* LoadedClass = ItemClassPath.TryLoadClass<ATurret>();
        if (LoadedClass)
        {
            TurretClassToSpawn = LoadedClass;
            UE_LOG(LogTemp, Warning, TEXT("GameManager : Success to load TurretClass!"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("GameManager : FAILED to load TurretClass!"));
        }
    }
}

void UGameManager::BeginPreparationPhase()
{
    if (CurrentState == EGameState::EGS_GameOver) return;

    CurrentWaveNumber++;

    if (CurrentWaveNumber > 10)
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        ATaskPlayer* TaskPlayer = Cast<ATaskPlayer>(PlayerPawn);
        if (TaskPlayer)
        {
            GetCumulativeKills();

            TaskPlayer->GameOver(false);
        }
    }

    CurrentState = EGameState::EGS_Preparing;

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

bool UGameManager::SpawnTurret()
{
    if (CurrentTurretNum > MaxTurretNum)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnTurret FAILED: Turret Is Enonugh"));
        return false;
    }
    
    if (!TurretClassToSpawn)
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnTurret FAILED: TurretClassToSpawn is NULL."));
        return false;
    }

    if (TurretSpawnTransforms.IsValidIndex(CurrentTurretNum))
    {
        FTransform SpawnTransform = TurretSpawnTransforms[CurrentTurretNum];

        GetWorld()->SpawnActor<ATurret>(TurretClassToSpawn, SpawnTransform);

        UE_LOG(LogTemp, Log, TEXT("Turret spawned at index %d"), CurrentTurretNum);

        CurrentTurretNum++;

        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("SpawnTurret FAILED: Invalid SpawnPointIndex %d"), CurrentTurretNum);
    return false;
}

void UGameManager::GetRewardBioCoin(int32 value)
{
    if (value > 0)
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        ATaskPlayer* TaskPlayer = Cast<ATaskPlayer>(PlayerPawn);
        if (TaskPlayer)
        {
            UInventoryComponent* PlayerInventory = TaskPlayer->FindComponentByClass<UInventoryComponent>();
            if (PlayerInventory)
            {
                PlayerInventory->AddBioCoreCount(value);

                UE_LOG(LogTemp, Log, TEXT("Awarded %d BioCore to Player for clearing wave."), value);
            }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("Inventory is null"), value);
            }
        }
        else
        {
            UE_LOG(LogTemp, Log, TEXT("Player Ref is null"), value);
        }
    }
}

void UGameManager::HandleEnemyKilled(ABaseCharacter* DiedEnemy)
{
    if (DiedEnemy)
    {
        CumulativeKills++;

        UE_LOG(LogTemp, Log, TEXT("Total Kills: %d"), CumulativeKills);
    }
}

void UGameManager::UpgradeDoorBattery()
{
    for (ADoorControlPanel* Panel : AllControlPanelsInLevel)
    {
        if (Panel)
        {
            Panel->UpgradeBattery();
        }
    }
}