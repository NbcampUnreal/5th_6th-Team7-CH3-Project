// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ControllableDoor.generated.h"

UCLASS()
class ZWAVE_API AControllableDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	AControllableDoor();

    UFUNCTION(BlueprintCallable, Category = "Door Control")
    void LockDoor();

    UFUNCTION(BlueprintCallable, Category = "Door Control")
    void UnlockDoor();

    UFUNCTION(BlueprintPure, Category = "Door Control")
    bool IsBusy() const { return bIsTransitioning; }

    UFUNCTION(BlueprintPure, Category = "Door Control")
    float GetDoorAnimationTime() const { return DoorAnimationTime; }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<USceneComponent> Root;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> DoorFrameMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> DoorMesh;

    UPROPERTY(EditAnywhere, Category = "Door Control")
    float DoorAnimationTime = 1.0f;

    FTimerHandle DoorAnimationTimerHandle;

    UPROPERTY(VisibleAnywhere, Category = "Door Control")
    bool bIsClosed = false;

    UPROPERTY(VisibleAnywhere, Category = "Door Control")
    bool bIsTransitioning = false;

    UFUNCTION()
    void UpdateDoorStateVisuals();

    UFUNCTION(BlueprintCallable)
    void OnAnimationFinished();

    UFUNCTION(BlueprintImplementableEvent, Category = "Door Control")
    void PlayDoorAnimation(bool bIsOpening);
};
