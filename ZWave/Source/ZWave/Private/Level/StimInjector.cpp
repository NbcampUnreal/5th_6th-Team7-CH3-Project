// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/StimInjector.h"
#include "Components/SphereComponent.h"
#include "Level/GameManager.h"
#include "Player/TaskPlayer.h"

AStimInjector::AStimInjector()
{
	PrimaryActorTick.bCanEverTick = false;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    InjectorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InjectorMesh"));
    InjectorMesh->SetupAttachment(Root);

    InteractSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));
    InteractSphere->SetupAttachment(Root);
    InteractSphere->SetSphereRadius(200.0f);
    InteractSphere->SetCollisionProfileName(TEXT("Trigger"));
    InteractSphere->OnComponentBeginOverlap.AddDynamic(this, &AStimInjector::OnSphereOverlapBegin);
    InteractSphere->OnComponentEndOverlap.AddDynamic(this, &AStimInjector::OnSphereOverlapEnd);
}

void AStimInjector::BeginPlay()
{
    Super::BeginPlay();

    GameManager = GetWorld()->GetSubsystem<UGameManager>();
    if (GameManager.IsValid())
    {
        GameManager->OnStimStateChanged.AddDynamic(this, &AStimInjector::OnGlobalStimStateChanged);

        OnGlobalStimStateChanged(GameManager->IsStimOnCooldown());
    }
}

void AStimInjector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (GameManager.IsValid())
    {
        GameManager->OnStimStateChanged.RemoveDynamic(this, &AStimInjector::OnGlobalStimStateChanged);
    }

    Super::EndPlay(EndPlayReason);
}

void AStimInjector::OnInteract_Implementation(AActor* Interactor)
{
    if (GameManager.IsValid())
    {
        GameManager->UseStim(Interactor);
    }
}

void AStimInjector::OnGlobalStimStateChanged(bool bIsNowOnCooldown)
{
    UpdateVisuals(!bIsNowOnCooldown);
}

void AStimInjector::OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ATaskPlayer* Player = Cast<ATaskPlayer>(OtherActor);
    if (Player)
    {
        //Player->SetCurrentInteractable(this);
        OnInteract_Implementation(Player); // 테스트용
        UE_LOG(LogTemp, Log, TEXT("Player entered interact range of Injector."));
    }
}

void AStimInjector::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    ATaskPlayer* Player = Cast<ATaskPlayer>(OtherActor);
    if (Player)
    {
        //Player->ClearCurrentInteractable(this);
        UE_LOG(LogTemp, Log, TEXT("Player left interact range of Injector."));
    }
}