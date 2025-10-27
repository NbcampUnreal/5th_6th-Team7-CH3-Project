// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemySpawnManager.generated.h"

class ABaseCharacter;
class ASpawnPoint;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyDiedSignature, ABaseCharacter*);

UCLASS()
class ZWAVE_API UEnemySpawnManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	void RegisterSpawnPoint(ASpawnPoint* Point);
	void UnregisterSpawnPoint(ASpawnPoint* Point);

	void RequestSpawn(TSubclassOf<ABaseCharacter> MonsterClass, int32 Count);

	FOnEnemyDiedSignature OnEnemyDied;

protected:
    UPROPERTY()
    TArray<ASpawnPoint*> SpawnPoints;

    UPROPERTY()

    TArray<ABaseCharacter*> ActiveEnemies;
    UFUNCTION()
    void HandleEnemyDied(AActor* DiedActor);

    ASpawnPoint* GetRandomSpawnPoint();
};
