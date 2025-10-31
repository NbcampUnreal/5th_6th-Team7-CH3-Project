// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDefinition.generated.h"

USTRUCT(BlueprintType)
struct FCommonWeaponStats
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float AttackPower;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	float AttackRate;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Stats")
	int ModingAllows;
};

/**
 * 
 */
UCLASS(BlueprintType)
class ZWAVE_API UWeaponDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeaponBase> WeaponClass;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponName;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponAttachSocket = "weapon_r";
};
