// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemWeaponInstance.h"
#include "Item/ItemModeInstance.h"


bool UItemWeaponInstance::AttachMod(UItemModeInstance* ModInstance)
{
    if (!ModInstance || !ItemDef)
    {
        return false; // 유효하지 않음
    }

    if (!HasEmptyModSlot())
    {
        return false; // 슬롯 부족
    }

    if (IsModAttached(ModInstance))
    {
        return false; // 이미 이 무기에 장착됨
    }

    if (ModInstance->IsEquipped())
    {
        return false; // 다른 무기에 장착됨
    }

    AttachedMods.Add(ModInstance);

    ModInstance->OnEquipped(this);

    return true;
}

void UItemWeaponInstance::DetachMod(UItemModeInstance* ModInstance)
{
    if (!ModInstance || !IsModAttached(ModInstance))
    {
        return;
    }

    AttachedMods.Remove(ModInstance);

    ModInstance->OnUnequipped();

}

bool UItemWeaponInstance::HasEmptyModSlot() const
{
    if (!ItemDef) return false;

    int32 MaxSlots = GetModeSlotLimit();
    int32 CurrentSlots = AttachedMods.Num();

    return CurrentSlots < MaxSlots;
}

bool UItemWeaponInstance::IsModAttached(UItemModeInstance* ModInstance) const
{
    if (!ModInstance) return false;
    return AttachedMods.Contains(ModInstance);
}