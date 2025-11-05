// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/ControllableDoor.h"
#include "TimerManager.h"

AControllableDoor::AControllableDoor()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
    DoorFrameMesh->SetupAttachment(Root);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(Root);

    bIsClosed = false;
}

void AControllableDoor::LockDoor()
{
    if (bIsClosed || bIsTransitioning) return;

    bIsClosed = true;
    UpdateDoorStateVisuals();
}

void AControllableDoor::UnlockDoor()
{
    if (!bIsClosed || bIsTransitioning) return;

    bIsClosed = false;
    UpdateDoorStateVisuals();
}

void AControllableDoor::UpdateDoorStateVisuals()
{
    if (bIsClosed)
    {
        PlayDoorAnimation(false);
    }
    else
    {
        PlayDoorAnimation(true);
    }

    bIsTransitioning = true;
    GetWorld()->GetTimerManager().ClearTimer(DoorAnimationTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(
        DoorAnimationTimerHandle,
        this,
        &AControllableDoor::OnAnimationFinished,
        DoorAnimationTime,
        false);
}

void AControllableDoor::OnAnimationFinished()
{
    bIsTransitioning = false;
}