// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Item/ItemDefinition.h"
#include "FieldItem/FieldItem.h"
#include "Effect/EffectBase.h"
#include "Level/ItemDropTable.h"
#include "ItemSpawnManager.generated.h"

class UEnemySpawnManager;
class UWaveManager;
class ABaseCharacter;
class UInventoryComponent;
class UItemDefinition;
struct FWaveDataInfo;

UCLASS()
class ZWAVE_API UItemSpawnManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
    UItemSpawnManager();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    void GenerateDropQueue(const FWaveDataInfo& WaveData, int32 TotalMonsterCount);

    UItemDefinition* GetItemDefinitionByIndex(int32 Index) const;


    UFUNCTION()
    void HandleEnemyDied(ABaseCharacter* DiedEnemy);

    UPROPERTY(VisibleAnywhere, Category = "Item Spawning")
    TSubclassOf<AFieldItem> FieldItemClass;

    UFUNCTION(BlueprintCallable)
    TArray<int32> GetDropQueue() { return CurrentWaveDropQueue; }

    UPROPERTY()
    TWeakObjectPtr<UEnemySpawnManager> EnemySpawnManager;

    UPROPERTY()
    TWeakObjectPtr<UWaveManager> WaveManager;

    UDataTable* DropTable;

private:

    UPROPERTY()
    TArray<int32> CurrentWaveDropQueue;

    const int32 INDEX_NONE = 0;
    const int32 INDEX_HEALTH = 1;
    const int32 INDEX_AMMO = 2;
    const int32 INDEX_MATERIAL_T1 = 3;
    const int32 INDEX_MATERIAL_T2 = 4;
    const int32 INDEX_MATERIAL_T3 = 5;
    const int32 INDEX_ACCESSORY_1 = 6;
    const int32 INDEX_ACCESSORY_2 = 7;
    const int32 INDEX_ACCESSORY_3 = 8; 

    int32 TotalMonstersThisWave = 0;
    int32 MonstersDiedThisWave = 0;

    const int32 MaxDropsPerMonster = 3;
};
