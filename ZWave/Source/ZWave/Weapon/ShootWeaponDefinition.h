// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDefinition.h"
#include "ShootWeaponDefinition.generated.h"


USTRUCT(BlueprintType)
struct FShootWeaponStats : public FCommonWeaponStats
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "ShootWeapon Stats")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, Category = "ShootWeapon Stats")
	bool bReloadAll;

	UPROPERTY(EditDefaultsOnly, Category = "ShootWeapon Stats")
	int Magazine;

	UPROPERTY(EditDefaultsOnly, Category = "ShootWeapon Stats")
	int SpareAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "ShootWeapon Stats")
	int ModingAllows;
};

/**
 * 
 */
UCLASS()
class ZWAVE_API UShootWeaponDefinition : public UWeaponDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FShootWeaponStats ShootWeaponStat;
};
