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

    StackCount = FMath::Clamp(NewCount, 0, MaxStack);
}

int32 UItemInstance::AddStack(int32 Amount)
{
    if (Amount <= 0) return 0;

    int32 MaxStack = GetMaxStackCount();

    int32 OldStack = StackCount;

    int32 NewStack = FMath::Clamp(OldStack + Amount, 0, MaxStack);

    StackCount = NewStack;

    return NewStack - OldStack;
}

void UItemInstance::RemoveStack(int32 Amount)
{
    if (Amount <= 0) return;

    SetStackCount(StackCount - Amount);
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