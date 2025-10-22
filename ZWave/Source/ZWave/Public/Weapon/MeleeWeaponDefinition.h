// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDefinition.h"
#include "MeleeWeaponDefinition.generated.h"

USTRUCT(BlueprintType)
struct FMeleeWeaponStats : public FCommonWeaponStats
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "MeleeWeapon Stats")
	float Radius;
};

/**
 * 
 */
UCLASS()
class ZWAVE_API UMeleeWeaponDefinition : public UWeaponDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FMeleeWeaponStats MeleeWeaponStat;
};
