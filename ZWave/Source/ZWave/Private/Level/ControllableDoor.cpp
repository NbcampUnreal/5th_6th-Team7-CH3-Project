// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/ControllableDoor.h"
#include "TimerManager.h"

#include "Components/AudioComponent.h"
AControllableDoor::AControllableDoor()
{
    PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    DoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
    DoorFrameMesh->SetupAttachment(Root);

    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorMesh->SetupAttachment(Root);

    DoorSoundAC = CreateDefaultSubobject<UAudioComponent>(TEXT("DoorSoundAC"));
    DoorSoundAC->SetupAttachment(GetRootComponent());
    DoorSoundAC->bAutoActivate = false;
    DoorSoundAC->bAutoDestroy = false;
    DoorSoundAC->SetUISound(false);
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
        PlayDoorSound();
        PlayDoorAnimation(false);
    }
    else
    {
        PlayDoorSound();
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

void AControllableDoor::PlayDoorSound()
{
    if (!DoorSoundAC || !DoorSound) return;

    DoorSoundAC->SetSound(DoorSound);    
    if (DoorSoundAttenuation)
        DoorSoundAC->AttenuationSettings = DoorSoundAttenuation;

    DoorSoundAC->Play(0.f);
}

void AControllableDoor::OnAnimationFinished()
{
    bIsTransitioning = false;
}