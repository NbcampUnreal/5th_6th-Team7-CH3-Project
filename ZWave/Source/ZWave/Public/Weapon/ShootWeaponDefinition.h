// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponDefinition.h"
#include "ShootWeaponDefinition.generated.h"

UENUM(BlueprintType)
enum class EShootType : uint8
{
	ST_None,
	ST_HandHun,
	ST_ShotHun,
	ST_Rifle,
};

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
};

/**
 * 
 */
UCLASS(BlueprintType)
class ZWAVE_API UShootWeaponDefinition : public UWeaponDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FShootWeaponStats ShootWeaponStat;

	UPROPERTY(EditDefaultsOnly)
	FName MuzzleSocketName = "Muzzle";

	UPROPERTY(EditDefaultsOnly)
	float TraceDistance = 10000.f;

	UPROPERTY(EditDefaultsOnly)
	EShootType ShootType;
};
