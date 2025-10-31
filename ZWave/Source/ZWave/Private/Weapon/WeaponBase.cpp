// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "GameFramework/Character.h"
#include "Mode/ModingInstance.h"

void AWeaponBase::Equip(ACharacter* NewOwner)
{
	OwningCharacter = NewOwner;
	bEquipped = true;
}

void AWeaponBase::Unequip()
{
	OwningCharacter = nullptr;
	bEquipped = false;
}

UModingInstance* AWeaponBase::GetModing(EModingSlot ModingSlot)
{
	if(EquipModingMap.Contains(ModingSlot) == false)
		return nullptr;

	return EquipModingMap[ModingSlot];
}
