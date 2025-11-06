// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/ItemSpawnManager.h"
#include "Level/EnemySpawnManager.h" 
#include "Level/WaveManager.h"    
#include "GameFramework/Character.h"  
#include "Item/InventoryComponent.h" 
#include "Item/ItemDefinition.h" 
#include "FieldItem/FieldItem.h"    
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "Level/ZWaveTypes.h" 
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

UItemSpawnManager::UItemSpawnManager()
{
    static ConstructorHelpers::FObjectFinder<UDataTable> DropTableFinder(TEXT("/Game/Data/DT_ItemDropTable.DT_ItemDropTable"));

    if (DropTableFinder.Succeeded())
    {
        this->DropTable = DropTableFinder.Object;
    }
}

void UItemSpawnManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    EnemySpawnManager = GetWorld()->GetSubsystem<UEnemySpawnManager>();
    WaveManager = GetWorld()->GetSubsystem<UWaveManager>();

    FieldItemClass = AFieldItem::StaticClass();

    FSoftClassPath ItemClassPath(TEXT("Blueprint'/Game/HyunJeongSeok/FieldItem/BP_FieldItem.BP_FieldItem_C'"));
    UClass* LoadedClass = ItemClassPath.TryLoadClass<AFieldItem>();
    if (LoadedClass)
    {
        FieldItemClass = LoadedClass;
        UE_LOG(LogTemp, Error, TEXT("ItemSpawnManager: Success to load FieldItemClass!"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ItemSpawnManager: FAILED to load FieldItemClass!"));
    }

    UE_LOG(LogTemp, Warning, TEXT("ItemSpawnManagerSubsystem Initialized"));

    if (EnemySpawnManager.IsValid())
    {
        EnemySpawnManager->OnEnemyDied.AddUObject(this, &UItemSpawnManager::HandleEnemyDied);
    }
}

void UItemSpawnManager::GenerateDropQueue(const FWaveDataInfo& WaveData, int32 TotalMonsterCount)
{
    CurrentWaveDropQueue.Empty();
    TotalMonstersThisWave = TotalMonsterCount;
    MonstersDiedThisWave = 0;


    if (TotalMonsterCount <= 0) return;

    for (int32 i = 0; i < WaveData.HealthPackCount; ++i)
    {
        CurrentWaveDropQueue.Add(INDEX_HEALTH);
    }
    for (int32 i = 0; i < WaveData.AmmoPackCount; ++i)
    {
        CurrentWaveDropQueue.Add(INDEX_AMMO);
    }
    for (int32 i = 0; i < WaveData.T1_Count; ++i)
    {
        CurrentWaveDropQueue.Add(INDEX_MATERIAL_T1);
    }
    for (int32 i = 0; i < WaveData.T2_Count; ++i)
    {
        CurrentWaveDropQueue.Add(INDEX_MATERIAL_T2);
    }
    for (int32 i = 0; i < WaveData.T3_Count; ++i)
    {
        CurrentWaveDropQueue.Add(INDEX_MATERIAL_T3);
    }

    if (WaveData.Accessory_Count > 0)
    {
        for (int32 i = 0; i < WaveData.Accessory_Count; ++i)
        {
            int32 RandomIndex = FMath::RandRange(0, 2);
            int32 AccessoryIndex = INDEX_ACCESSORY_1 + RandomIndex;
            CurrentWaveDropQueue.Add(AccessoryIndex);
        }
    }

    int32 LastIndex = CurrentWaveDropQueue.Num() - 1;
    for (int32 i = LastIndex; i > 0; --i)
    {
        int32 j = FMath::RandRange(0, i);

        CurrentWaveDropQueue.Swap(i, j);
    }
    UE_LOG(LogTemp, Log, TEXT("Drop Queue Generated. Total Items: %d (Monsters: %d)"), CurrentWaveDropQueue.Num(), TotalMonsterCount);
}



void UItemSpawnManager::HandleEnemyDied(ABaseCharacter* DiedEnemy)
{

    if (!DiedEnemy || !FieldItemClass || CurrentWaveDropQueue.Num() == 0)
    {
        return;
    }

    MonstersDiedThisWave++;

    int32 ItemsRemainingInQueue = CurrentWaveDropQueue.Num();
    int32 MonstersRemainingToSpawn = TotalMonstersThisWave - MonstersDiedThisWave;

    int32 MinGuaranteedDrops = ItemsRemainingInQueue - (MonstersRemainingToSpawn * MaxDropsPerMonster);

    int32 MaxPossibleDrops = FMath::Max(ItemsRemainingInQueue - MonstersRemainingToSpawn, 1);

    int32 NumToDropThisMonster = FMath::Clamp(MaxPossibleDrops, 1, MaxDropsPerMonster);
    if (MinGuaranteedDrops > NumToDropThisMonster)
    {
        NumToDropThisMonster = FMath::Clamp(MinGuaranteedDrops, 1, MaxDropsPerMonster);
    }

    UE_LOG(LogTemp, Warning, TEXT("Enemy Dead! Spawning %d items"), NumToDropThisMonster);

    for (int32 i = 0; i < NumToDropThisMonster; ++i)
    {

        if (CurrentWaveDropQueue.Num() == 0) break;

        int32 ItemIndex = CurrentWaveDropQueue.Pop();

        if (ItemIndex == INDEX_NONE) continue;

        FVector Location = DiedEnemy->GetActorLocation() + FVector(FMath::RandRange(-50, 50), FMath::RandRange(-50, 50), 30.0f);
        AFieldItem* SpawnedItem = GetWorld()->SpawnActor<AFieldItem>(FieldItemClass, Location, FRotator::ZeroRotator);
        if (!SpawnedItem) continue;

        SpawnedItem->Init(ItemIndex);
    }
}

UItemDefinition* UItemSpawnManager::GetItemDefinitionByIndex(int32 Index) const
{
    static const FString ContextString(TEXT("Find Item By Index"));
    TArray<FItemDropTable*> AllRows;

    DropTable->GetAllRows(ContextString, AllRows);

    for (FItemDropTable* Row : AllRows)
    {
        if (Row && Row->ItemIndex == Index)
        {
            return Row->ItemDefinition;
        }
    }

    return nullptr;
}

