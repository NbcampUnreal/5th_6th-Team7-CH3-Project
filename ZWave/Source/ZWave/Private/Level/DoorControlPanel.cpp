// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/DoorControlPanel.h"
#include "Level/ControllableDoor.h"
#include "Components/SphereComponent.h"
#include "Player/TaskPlayer.h"
#include "Level/GameManager.h"
#include "Engine/World.h"

ADoorControlPanel::ADoorControlPanel()
{
	PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PanelMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PanelMesh"));
	PanelMesh->SetupAttachment(Root);

    InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
    InteractSphere->SetupAttachment(Root);
    InteractSphere->SetSphereRadius(200.0f);
    InteractSphere->SetCollisionProfileName(TEXT("Trigger"));
    InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &ADoorControlPanel::OnSphereOverlapBegin);
    InteractSphere->OnComponentEndOverlap.AddDynamic(this, &ADoorControlPanel::OnSphereOverlapEnd);
}

void ADoorControlPanel::BeginPlay()
{
    Super::BeginPlay();
    CurrentBattery = MaxBattery;
    bIsLocked = false;
    bIsTransitioning = false;

    if (GetWorld())
    {
        GameManager = GetWorld()->GetSubsystem<UGameManager>();
        if (!GameManager.IsValid())
        {
            UE_LOG(LogTemp, Error, TEXT("DoorControlPanel: Failed to get GameManagerSubsystem!"));
        }
    }
}

void ADoorControlPanel::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    float OldBattery = CurrentBattery;

    if (bIsLocked && !bIsTransitioning)
    {
        float DrainAmount = BatteryDrainDoor * DeltaTime;
        CurrentBattery = FMath::Max(0.0f, CurrentBattery - DrainAmount);

        if (CurrentBattery == 0.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Panel battery depleted! Forcing unlock."));
            UnlockAllDoors();
        }
    }
    else if (!bIsLocked)
    {
        CurrentBattery = FMath::Min(MaxBattery, CurrentBattery + BatteryRegenRate * DeltaTime);
    }

    if (OldBattery != CurrentBattery)
    {
        OnPanelBatteryChanged.Broadcast(CurrentBattery / MaxBattery);
    }
}

void ADoorControlPanel::OnInteract_Implementation(AActor* Interactor)
{
    if (GameManager.IsValid() && GameManager->GetIsBlackOutActive())
    {
        UE_LOG(LogTemp, Warning, TEXT("Panel: Cannot operate during blackout!"));
        return;
    }
    
    if (bIsTransitioning)
    {
        UE_LOG(LogTemp, Log, TEXT("Panel is busy (waiting for doors to finish moving)."));
        return;
    }

    if (!AreAllDoorsIdle())
    {
        UE_LOG(LogTemp, Log, TEXT("Panel: Cannot operate, one or more doors are busy."));
        return;
    }

    if (bIsLocked)
    {
        UnlockAllDoors();
    }
    else
    {
        if (CurrentBattery > 0.0f)
        {
            LockAllDoors();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Panel: Battery is dead. Cannot lock."));
        }
    }
}

void ADoorControlPanel::LockAllDoors()
{
    if (TargetDoors.Num() == 0 || bIsLocked) return;

    UE_LOG(LogTemp, Log, TEXT("Panel: Locking all doors..."));
    bIsLocked = true;
    bIsTransitioning = true;
    OnPanelStateChanged.Broadcast(true);

    for (AControllableDoor* Door : TargetDoors)
    {
        if (Door)
        {
            Door->LockDoor();
        }
    }

    GetWorld()->GetTimerManager().SetTimer(
        DoorAnimationTimerHandle,
        this,
        &ADoorControlPanel::OnDoorsFinishedMoving,
        GetLongestDoorAnimationTime(),
        false);
}

void ADoorControlPanel::UnlockAllDoors()
{
    if (TargetDoors.Num() == 0 || !bIsLocked) return;

    UE_LOG(LogTemp, Log, TEXT("Panel: Unlocking all doors..."));
    bIsLocked = false;
    bIsTransitioning = true;
    OnPanelStateChanged.Broadcast(false);

    for (AControllableDoor* Door : TargetDoors)
    {
        if (Door)
        {
            Door->UnlockDoor();
        }
    }

    GetWorld()->GetTimerManager().SetTimer(
        DoorAnimationTimerHandle,
        this,
        &ADoorControlPanel::OnDoorsFinishedMoving,
        GetLongestDoorAnimationTime(),
        false);
}

bool ADoorControlPanel::AreAllDoorsIdle() const
{
    for (AControllableDoor* Door : TargetDoors)
    {
        if (Door && Door->IsBusy())
        {
            return false;
        }
    }
    return true;
}

float ADoorControlPanel::GetLongestDoorAnimationTime() const
{
    float MaxTime = 0.0f;
    for (AControllableDoor* Door : TargetDoors)
    {
        if (Door)
        {
            MaxTime = FMath::Max(MaxTime, Door->GetDoorAnimationTime());
        }
    }
    return MaxTime + 0.1f;
}

void ADoorControlPanel::OnDoorsFinishedMoving()
{
    bIsTransitioning = false;
    UE_LOG(LogTemp, Log, TEXT("Panel: All doors finished moving. Ready for next command."));
}

void ADoorControlPanel::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATaskPlayer* Player = Cast<ATaskPlayer>(OtherActor);
    if (Player)
    {
        //Player->SetCurrentInteractable(this);
        //OnInteract_Implementation(Player); // 테스트용
        Player->AddActiveObject(this);
        UE_LOG(LogTemp, Log, TEXT("Player entered interact range of Panel."));
    }
}

void ADoorControlPanel::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ATaskPlayer* Player = Cast<ATaskPlayer>(OtherActor);
    if (Player)
    {
        //Player->ClearCurrentInteractable(this);
        Player->RemoveActiveObject(this);
        UE_LOG(LogTemp, Log, TEXT("Player left interact range of Panel."));
    }
}

void ADoorControlPanel::UpgradeBattery(float time)
{

}