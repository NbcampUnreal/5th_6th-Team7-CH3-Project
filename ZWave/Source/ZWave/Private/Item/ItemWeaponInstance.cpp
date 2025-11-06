// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemWeaponInstance.h"
#include "Item/ItemModeInstance.h"
#include "Item/ItemDefinition.h"

void UItemWeaponInstance::Initialize(UItemDefinition* Definition, int32 Count)
{
    Super::Initialize(Definition, Count);

    if (Definition != nullptr)
    {
        AttachedMods.SetNum(Definition->ModingAllows);
    }
}

bool UItemWeaponInstance::AttachMod(UItemModeInstance* ModInstance)
{
    if (!ModInstance || !ItemDef)
    {
        return false; // 유효하지 않음
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

bool UItemWeaponInstance::AttachModSlot(UItemModeInstance* ModeInstance, int32 ModingSlot)
{
    if (!ModeInstance || !ItemDef)
    {
        return false; // 유효하지 않음
    }

    if (IsModAttached(ModeInstance))
    {
        return false; // 이미 이 무기에 장착됨
    }

    if (ModeInstance->IsEquipped())
    {
        return false; // 다른 무기에 장착됨
    }

    if (ModingSlot < 0 ||
        ModingSlot >= AttachedMods.Num())
    {
        return false;
    }

    AttachedMods[ModingSlot] = ModeInstance;
    ModeInstance->OnEquipped(this);

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

bool UItemWeaponInstance::IsModAttached(UItemModeInstance* ModInstance) const
{
    if (!ModInstance) return false;
    return AttachedMods.Contains(ModInstance);
}