// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/ClimbingPoint.h"
#include "Base/ZWaveGameState.h"
#include "Components/BoxComponent.h"

// Sets default values
AClimbingPoint::AClimbingPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    BoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleRoot"));
    SetRootComponent(BoxRoot);
}

void AClimbingPoint::BeginPlay()
{
    Super::BeginPlay();
    AZWaveGameState* GameState = GetWorld()->GetGameState<AZWaveGameState>();
    if (GameState)
    {
        GameState->RegisterClimbingPoint(this);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("SpawnPoint: Could not find ZWaveGameState!"));
    }
}
