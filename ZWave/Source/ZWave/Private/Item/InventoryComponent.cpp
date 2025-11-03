// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/InventoryComponent.h"
#include "Item/ItemWeaponInstance.h"
#include "Item/ItemModeInstance.h"
#include "Mode/ModeDefinition.h"
#include "Mode/ModingInstance.h"
#include "Weapon/WeaponBase.h"

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

bool UInventoryComponent::EquipWeaponItem(const UItemDefinition* ItemDef, EEquipSlot EquipSlot)
{
	if (GetOwner() == nullptr)
		return false;

	int32 InvenSlot = FindItem(ItemDef);
	if (InvenSlot == INDEX_NONE)
		return false;

	UItemInstance* TargetItem = Entries[InvenSlot].ItemInstance;
	if (TargetItem == nullptr)
		return false;

	UItemWeaponInstance* WeaponItem = Cast<UItemWeaponInstance>(TargetItem);
	if (WeaponItem == nullptr)
		return false;

	UEquipComponent* EquipComp = GetOwner()->FindComponentByClass<UEquipComponent>();
	if (EquipComp == nullptr)
		return false;

	if (WeaponItem->ItemDef == nullptr ||
		WeaponItem->ItemDef->Definition == nullptr)
		return false;

	UWeaponDefinition* WeaponDef = Cast<UWeaponDefinition>(WeaponItem->ItemDef->Definition);
	if (WeaponDef == nullptr)
		return false;

	EquipComp->SetSlotData(EquipSlot, WeaponDef);
	WeaponItem->EquipSlot = EquipSlot;

	// 모딩장착한 것이 있다면 WeaponActor에도 전달
	if(WeaponItem->AttachedMods.Num() > 0)
		EquipModingOnWeaponActor(WeaponItem, EquipSlot);

	return true;
}

bool UInventoryComponent::UnequipWeaponItem(const UItemDefinition* ItemDef, EEquipSlot EquipSlot)
{
	if (GetOwner() == nullptr)
		return false;

	int32 InvenSlot = FindItem(ItemDef);
	if (InvenSlot == INDEX_NONE)
		return false;

	UItemInstance* TargetItem = Entries[InvenSlot].ItemInstance;
	if (TargetItem == nullptr)
		return false;

	UItemWeaponInstance* WeaponItem = Cast<UItemWeaponInstance>(TargetItem);
	if (WeaponItem == nullptr)
		return false;

	if (WeaponItem->EquipSlot == EEquipSlot::None)
		return false;

	UEquipComponent* EquipComp = GetOwner()->FindComponentByClass<UEquipComponent>();
	if (EquipComp == nullptr)
		return false;

	if (WeaponItem->ItemDef == nullptr ||
		WeaponItem->ItemDef->Definition == nullptr)
		return false;

	UWeaponDefinition* WeaponDef = Cast<UWeaponDefinition>(WeaponItem->ItemDef->Definition);
	if (WeaponDef == nullptr)
		return false;

	EquipComp->UnEquip(WeaponItem->EquipSlot);
	EquipComp->ClearSlotData(WeaponItem->EquipSlot);
	WeaponItem->EquipSlot = EEquipSlot::None;

	return true;
}

TArray<int32> UInventoryComponent::GetWeaponItemSlotIdxs() const
{
	TArray<int32> SlotIdx;

	int32 NowInvenSize = Entries.Num();

	for (int32 i = 0; i < NowInvenSize; ++i)
	{
		if (Entries[i].IsEmpty())
			continue;

		UItemInstance* TargetItem = Entries[i].ItemInstance;
		if (TargetItem == nullptr)
			continue;

		UItemWeaponInstance* WeaponItem = Cast<UItemWeaponInstance>(TargetItem);
		if (WeaponItem == nullptr)
			continue;

		SlotIdx.Add(i);
	}

	return SlotIdx;
}

bool UInventoryComponent::EquipModingToWeapon(int32 TargetWeaponSlotIdx, int32 TargetModingSlotIdx)
{
	if (GetOwner() == nullptr)
		return false;

	if (TargetWeaponSlotIdx < 0 ||
		TargetWeaponSlotIdx >= MaxEntryCount ||
		TargetModingSlotIdx < 0 || 
		TargetModingSlotIdx >= MaxEntryCount)
		return false;

	UItemInstance* TargetWeaponItem = Entries[TargetWeaponSlotIdx].ItemInstance;
	if (TargetWeaponItem == nullptr)
		return false;

	UItemWeaponInstance* WeaponItem = Cast<UItemWeaponInstance>(TargetWeaponItem);
	if (WeaponItem == nullptr)
		return false;

	UItemInstance* TargetModingItem = Entries[TargetModingSlotIdx].ItemInstance;
	if (TargetModingItem == nullptr)
		return false;

	UItemModeInstance* ModingItem = Cast<UItemModeInstance>(TargetModingItem);
	if (ModingItem == nullptr)
		return false;

	if (ModingItem->IsEquipped() == true ||
		WeaponItem->IsModAttached(ModingItem))
	{
		UE_LOG(LogTemp, Warning, TEXT("Moding Already Attached!"));
		return false;
	}

	WeaponItem->AttachMod(ModingItem);

	if (WeaponItem->EquipSlot != EEquipSlot::None)
	{
		EquipModingOnWeaponActor(WeaponItem, WeaponItem->EquipSlot);
	}

	return true;
}

bool UInventoryComponent::UnequipModingToWeapon(int32 TargetWeaponSlotIdx, int32 TargetModingSlotIdx)
{
	if (GetOwner() == nullptr)
		return false;

	if (TargetWeaponSlotIdx < 0 ||
		TargetWeaponSlotIdx >= MaxEntryCount ||
		TargetModingSlotIdx < 0 ||
		TargetModingSlotIdx >= MaxEntryCount)
		return false;

	UItemInstance* TargetWeaponItem = Entries[TargetWeaponSlotIdx].ItemInstance;
	if (TargetWeaponItem == nullptr)
		return false;

	UItemWeaponInstance* WeaponItem = Cast<UItemWeaponInstance>(TargetWeaponItem);
	if (WeaponItem == nullptr)
		return false;

	UItemInstance* TargetModingItem = Entries[TargetModingSlotIdx].ItemInstance;
	if (TargetModingItem == nullptr)
		return false;

	UItemModeInstance* ModingItem = Cast<UItemModeInstance>(TargetModingItem);
	if (ModingItem == nullptr)
		return false;

	if (ModingItem->IsEquipped() == false ||
		WeaponItem->IsModAttached(ModingItem) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Equipped Moding!"));
		return false;
	}

	WeaponItem->DetachMod(ModingItem);

	if (WeaponItem->EquipSlot != EEquipSlot::None)
	{
		EquipModingOnWeaponActor(WeaponItem, WeaponItem->EquipSlot);
	}

	return true;
}

void UInventoryComponent::EquipModingOnWeaponActor(UItemWeaponInstance* WeaponItem, EEquipSlot EquipSlot)
{
	if (WeaponItem == nullptr)
		return;

	if (GetOwner() == nullptr)
		return;

	UEquipComponent* EquipComp = GetOwner()->FindComponentByClass<UEquipComponent>();
	if (EquipComp == nullptr)
		return;

	AWeaponBase* WeaponActor = EquipComp->GetTargetWeapon(EquipSlot);
	if (WeaponActor == nullptr)
		return;

	for (int32 i = 0; i < WeaponItem->AttachedMods.Num(); ++i)
	{
		UItemModeInstance* AttachedMod = WeaponItem->AttachedMods[i];

		if (AttachedMod == nullptr ||
			AttachedMod->ItemDef == nullptr)
			continue;

		UModeDefinition* ModeDef = Cast<UModeDefinition>(AttachedMod->ItemDef);
		if (ModeDef == nullptr)
			continue;

		EModingSlot ModingSlot = static_cast<EModingSlot>(i);
		UModingInstance* ModingInstance = NewObject<UModingInstance>(WeaponActor);
		if (ModingInstance == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Mode Instance Create Failed!?"));
			continue;
		}

		ModingInstance->Init(ModeDef);
		WeaponActor->EquipModing(ModingSlot, ModingInstance);
	}
}
