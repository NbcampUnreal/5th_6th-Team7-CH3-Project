// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemModeInstance.h"
#include "Item/ItemWeaponInstance.h"

bool UItemModeInstance::IsEquipped() const
{
    return EquippedWeapon != nullptr;
}

void UItemModeInstance::OnEquipped(UItemWeaponInstance* TargetWeapon)
{
    EquippedWeapon = TargetWeapon;
}

void UItemModeInstance::OnUnequipped()
{
    EquippedWeapon = nullptr;
}