// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/MeleeWeaponDefinition.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API AMeleeWeapon : public AWeaponBase
{
	GENERATED_BODY()
public:
	virtual void Attack() override;

	virtual bool Init(const UWeaponDefinition* WeaponDefinition) override;

	virtual void Equip(ACharacter* NewOwner) override;

	virtual void Unequip() override;

protected:
	FMeleeWeaponStats MeleeWeaponStat;
};
