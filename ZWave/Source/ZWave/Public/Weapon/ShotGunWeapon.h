// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ShootWeapon.h"
#include "ShotGunWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API AShotGunWeapon : public AShootWeapon
{
	GENERATED_BODY()
public:
	virtual void Attack() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	int32 ShotGunBulletCount;

	UPROPERTY(EditDefaultsOnly)
	float ShotGunBulletDegree;
};
