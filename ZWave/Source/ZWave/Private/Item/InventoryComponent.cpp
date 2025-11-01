// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InventoryComponent.h"

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Entries.SetNum(MaxEntryCount);
}

void UInventoryComponent::AddItem(UItemDefinition* ItemDef, int32 Quantity)
{
	if (ItemDef == nullptr
		|| Quantity <= 0)
		return;

	int32 Remaining = Quantity;

	for (int32 i = 0; i < Entries.Num() && Remaining > 0; ++i)
	{
		FInventoryEntry& Entry = Entries[i];
		if (Entry.IsEmpty() == false && Entry.ItemInstance->ItemDef == ItemDef)
		{
			// 현재 스택이 가득이면 pass
			const int32 MaxStack = ItemDef->MaxStack;
			const int32 Cur = Entry.ItemInstance->StackCount;
			if (Cur >= MaxStack)
				continue;

			const int32 Space = MaxStack - Cur;
			const int32 ToAdd = FMath::Min(Space, Remaining);

			Entry.ItemInstance->AddStack(ToAdd);
			Remaining -= ToAdd;

			OnInventoryUpdated.Broadcast(Entry);
		}
	}

	for (int32 i = 0; i < Entries.Num() && Remaining > 0; ++i)
	{
		FInventoryEntry& Entry = Entries[i];
		if (Entry.IsEmpty())
		{
			const int32 ToCreate = FMath::Min(ItemDef->MaxStack, Remaining);

			UItemInstance* NewItem = NewObject<UItemInstance>(this);
			NewItem->Initialize(ItemDef, ToCreate);

			Entry.ItemInstance = NewItem;
			Remaining -= ToCreate;

			OnInventoryUpdated.Broadcast(Entry);
		}
	}

	if (Remaining > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory Is Full!! Remaining: %d"), Remaining);
	}
}

bool UInventoryComponent::RemoveItem(const UItemDefinition* ItemDef, int32 Quantity)
{
	if (ItemDef == nullptr
		|| Quantity <= 0)
		return false;

	const int32 Owned = CountItem(ItemDef);
	if (Owned < Quantity)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not enough items: %s (Have %d, Need %d)"),
			*ItemDef->GetName(), Owned, Quantity);
		return false;
	}

	int32 Remaining = Quantity;

	for (int32 i = 0; i < Entries.Num() && Remaining > 0; ++i)
	{
		FInventoryEntry& Entry = Entries[i];
		if (Entry.IsEmpty() || Entry.ItemInstance->ItemDef != ItemDef)
			continue;

		const int32 CurCount = Entry.ItemInstance->StackCount;
		const int32 ToRemove = FMath::Min(CurCount, Remaining);

		Entry.ItemInstance->StackCount -= ToRemove;
		Remaining -= ToRemove;

		if (Entry.ItemInstance->StackCount <= 0)
		{
			Entry.ItemInstance = nullptr;
		}

		OnInventoryUpdated.Broadcast(Entry);
	}

	return true;
}


int32 UInventoryComponent::FindItem(const UItemDefinition* ItemDef) const
{
	if (ItemDef == nullptr)
	{
		return INDEX_NONE;
	}

	for (int32 i = 0; i < Entries.Num(); ++i)
	{
		const FInventoryEntry& E = Entries[i];
		if (E.IsEmpty() == false
			&& E.ItemInstance->ItemDef == ItemDef)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UInventoryComponent::CountItem(const UItemDefinition* ItemDef) const
{
	if (ItemDef == nullptr)
	{
		return 0;
	}

	int32 Total = 0;
	for (const FInventoryEntry& E : Entries)
	{
		if (E.IsEmpty() == false &&
			E.ItemInstance->ItemDef == ItemDef)
		{
			Total += E.ItemInstance->StackCount;
		}
	}
	return Total;
}
