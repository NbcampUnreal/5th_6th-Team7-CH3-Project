// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponDefinition.h"
#include "NiagaraSystem.h"
#include "ProjectileWeaponDefinition.generated.h"

USTRUCT(BlueprintType)
struct FProjectileWeaponStats : public FCommonWeaponStats
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon Stats")
	float Radius;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon Stats")
	float FuseTime;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon Stats")
	float InitialSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon Stats")
	UNiagaraSystem* ExplosionVFX;

	UPROPERTY(EditDefaultsOnly, Category = "ProjectileWeapon Stats")
	int32 ProjectileIdx;
};

/**
 * 
 */
UCLASS()
class ZWAVE_API UProjectileWeaponDefinition : public UWeaponDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FProjectileWeaponStats ProjectileWeaponStat;
};
