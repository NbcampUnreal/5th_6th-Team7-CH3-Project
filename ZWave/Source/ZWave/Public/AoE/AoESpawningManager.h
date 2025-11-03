// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AoE/AoEData.h"
#include "AoESpawningManager.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UAoESpawningManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(EditDefaultsOnly, Category = "AoEData")
	TArray<UAoEData*> AoEDataArray;

	UPROPERTY(EditDefaultsOnly, Category = "AoEData")
	TSubclassOf<class AAoEActor> SpawnActorClass;

	UFUNCTION(BlueprintCallable, Category = "GetAoEData")
	bool SpawnAoEActor(AActor* Causer, int32 index, FVector SpawnPoint);

	UFUNCTION(BlueprintCallable, Category = "AddAoEData")
	void AddAoEData();
	
};
