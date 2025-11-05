// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/InteractInterface.h"
#include "StimInjector.generated.h"

class USphereComponent;
class UGameManager;

UCLASS()
class ZWAVE_API AStimInjector : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	AStimInjector();

	virtual void OnInteract_Implementation(AActor* Interactor) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> InjectorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> InteractSphere;

	UFUNCTION(BlueprintImplementableEvent, Category = "Stim")
	void UpdateVisuals(bool bIsNowAvailable);

	UFUNCTION()
	void OnSphereOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION()
	void OnGlobalStimStateChanged(bool bIsNowOnCooldown);

	UPROPERTY()
	TWeakObjectPtr<UGameManager> GameManager;
};
