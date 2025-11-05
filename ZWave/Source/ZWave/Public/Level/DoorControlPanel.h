// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/InteractInterface.h"
#include "DoorControlPanel.generated.h"

class AControllableDoor;
class USphereComponent;
class UGameManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPanelBatteryChanged, float, BatteryPercentage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPanelStateChanged, bool, bIsNowLocked);

UCLASS()
class ZWAVE_API ADoorControlPanel : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	ADoorControlPanel();

    virtual void OnInteract_Implementation(AActor* Interactor) override;

    virtual void Tick(float DeltaTime) override;

    UPROPERTY(BlueprintAssignable, Category = "Panel Events")
    FOnPanelBatteryChanged OnPanelBatteryChanged;

    UPROPERTY(BlueprintAssignable, Category = "Panel Events")
    FOnPanelStateChanged OnPanelStateChanged;

    void UnlockAllDoors();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> PanelMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USphereComponent> InteractSphere;

    UPROPERTY(EditInstanceOnly, Category = "Door Control")
    TArray<TObjectPtr<AControllableDoor>> TargetDoors;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Panel State")
    bool bIsLocked = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Panel State")
    bool bIsTransitioning = false;

    UPROPERTY(EditDefaultsOnly, Category = "Panel State")
    float MaxBattery = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Panel State")
    float CurrentBattery;

    UPROPERTY(EditDefaultsOnly, Category = "Panel State")
    float BatteryDrainDoor = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Panel State")
    float BatteryRegenRate = 10.0f;

    UPROPERTY(Transient)
    TWeakObjectPtr<UGameManager> GameManager;

    FTimerHandle DoorAnimationTimerHandle;

    UFUNCTION()
    void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION()
    void OnDoorsFinishedMoving();

    void LockAllDoors();

    bool AreAllDoorsIdle() const;

    float GetLongestDoorAnimationTime() const;



};
