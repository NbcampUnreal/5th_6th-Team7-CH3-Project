// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "AoEData.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UAoEData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AoE")
	int32 AoEDataIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AoE")
	float DamagePerSec;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AoE")
	float ActiveTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AoE")
	UNiagaraSystem* NiagaraParicle;
};
