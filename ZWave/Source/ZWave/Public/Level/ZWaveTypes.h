// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ZWaveTypes.generated.h"

class ABaseCharacter;

UENUM(BlueprintType)
enum class EGameState : uint8
{
    EGS_None        UMETA(DisplayName = "None"),
    EGS_Preparing   UMETA(DisplayName = "Preparing Wave"),
    EGS_WaveInProgress UMETA(DisplayName = "Wave In Progress"),
    EGS_GameOver    UMETA(DisplayName = "Game Over")
};

USTRUCT(BlueprintType)
struct FMonsterSpawnInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterSpawn")
    TSubclassOf<ABaseCharacter> MonsterClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterSpawn")
    int32 NumberToSpawn;
};

USTRUCT(BlueprintType)
struct FWaveDataInfo : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 WaveNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    TArray<FMonsterSpawnInfo> MonsterSpawnList;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wave")
    int32 WaveClearRewardBioCoin;
};