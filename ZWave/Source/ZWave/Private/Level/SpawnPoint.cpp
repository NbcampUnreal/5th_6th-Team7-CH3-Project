// Copyright Epic Games, Inc. All Rights Reserved.

#include "Level/SpawnPoint.h"
#include "Components/ArrowComponent.h"
#include "Engine/World.h"
#include "Level/EnemySpawnManager.h"

ASpawnPoint::ASpawnPoint()
{
    PrimaryActorTick.bCanEverTick = false;

    SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));

    ArrowVisualizer = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowVisualizer"));

    ArrowVisualizer->SetupAttachment(GetRootComponent());
    ArrowVisualizer->ArrowColor = FColor::Red;
    ArrowVisualizer->ArrowSize = 2.0f;
}

void ASpawnPoint::BeginPlay()
{
    Super::BeginPlay();

    UEnemySpawnManager* SpawnManager = GetWorld()->GetSubsystem<UEnemySpawnManager>();
    if (SpawnManager)
    {
        SpawnManager->RegisterSpawnPoint(this);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnPoint: Could not find EnemySpawnManagerSubsystem!"));
    }
}

void ASpawnPoint::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (GetWorld())
    {
        UEnemySpawnManager* SpawnManager = GetWorld()->GetSubsystem<UEnemySpawnManager>();
        if (SpawnManager)
        {
            SpawnManager->UnregisterSpawnPoint(this);
        }
    }

    Super::EndPlay(EndPlayReason);
}