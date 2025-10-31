// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemInstance.h"
#include "Item/ItemDefinition.h"

void UItemInstance::Initialize(UItemDefinition* Definition, int32 Count)
{
    ItemDef = Definition;
    SetStackCount(Count);
}

void UItemInstance::SetStackCount(int32 NewCount)
{
    int32 MaxStack = GetMaxStackCount();
    MaxStackCount = FMath::Clamp(NewCount, 0, MaxStack);
}

int32 UItemInstance::AddStack(int32 Amount)
{
    if (Amount <= 0) return 0;

    int32 MaxStack = GetMaxStackCount();
    int32 OldStack = MaxStackCount;
    int32 NewStack = FMath::Clamp(OldStack + Amount, 0, MaxStack);

    MaxStackCount = NewStack;
    return NewStack - OldStack; // 실제로 몇 개가 추가되었는지 반환
}

void UItemInstance::RemoveStack(int32 Amount)
{
    if (Amount <= 0) return;
    SetStackCount(MaxStackCount - Amount);
}

int32 UItemInstance::GetMaxStackCount() const
{
    if (ItemDef == nullptr)
    {
        return 0;
    }

    if (ItemDef->ItemType == EItemType::EIT_Material || ItemDef->ItemType == EItemType::EIT_Projectile)
    {
        return ItemDef->MaxStack;
    }

    return 1;
}

int32 UItemInstance::GetModeSlotLimit() const
{
    return ItemDef->ModSlotLimit;
}

