// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Level/ZWaveTypes.h"
#include "GameFramework/Character.h"
#include "EnemySpawnManager.generated.h"

class ABaseCharacter;
class ASpawnPoint;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyDiedSignature, ABaseCharacter*);

USTRUCT()
struct FSpawnRequest
{
    GENERATED_BODY()

    UPROPERTY()
    TSubclassOf<ABaseCharacter> MonsterClass;

    UPROPERTY()
    int32 Count;

    FSpawnRequest()
        : MonsterClass(nullptr), Count(0) {
    }

    FSpawnRequest(TSubclassOf<ABaseCharacter> InClass, int32 InCount)
        : MonsterClass(InClass), Count(InCount) {
    }
};

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
    TArray<ABaseEnemy*> ActiveEnemies;

	UPROPERTY()
	TArray<FSpawnRequest> SpawnQueue;

    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    int32 SpawnsPerTick = 5;

    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    float SpawnInterval = 1.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Spawning")
    int32 MaxActiveEnemies = 30;

	FTimerHandle SpawnTimerHandle;

    UFUNCTION()
    void HandleEnemyDied(AActor* DiedActor);

	UFUNCTION()
	void OnSpawnTimerTick();

    ASpawnPoint* GetRandomSpawnPoint();
};
