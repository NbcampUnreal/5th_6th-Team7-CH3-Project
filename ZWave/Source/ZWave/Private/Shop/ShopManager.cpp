// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop/ShopManager.h"
#include "Item/InventoryComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogShop, Log, All);

// ──────────────────────────────────────────────────────────────
// 내부 헬퍼: Player → Character/Inventory/Equip 접근
// ──────────────────────────────────────────────────────────────

static ACharacter* GetPlayerCharacterChecked(APlayerController* PC)
{
	return PC ? Cast<ACharacter>(PC->GetPawn()) : nullptr;
}

static UInventoryComponent* GetInventoryFromPlayer(APlayerController* PC)
{
	if (ACharacter* Char = GetPlayerCharacterChecked(PC))
	{
		return Char->FindComponentByClass<UInventoryComponent>();
	}
	return nullptr;
}

// ──────────────────────────────────────────────────────────────
// UShopManager 구현
// ──────────────────────────────────────────────────────────────

bool UShopManager::TryPurchaseItem(APlayerController* Player, FShopItemData& ItemData)
{
	if (Player == nullptr
		|| ItemData.ItemDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryPurchaseItem: invalid args"));
		return false;
	}

	if (HasEnoughCore(Player, ItemData.ItemDef->BuyPrice) == false)
	{
		UE_LOG(LogTemp, Log, TEXT("TryPurchaseItem: not enough money. Need %d"), ItemData.ItemDef->BuyPrice);
		return false;
	}

	DeductCore(Player, ItemData.ItemDef->BuyPrice);
	GiveItemToInventory(Player, ItemData.ItemDef);

	if (ItemData.Stock > 0)
		ItemData.Stock--;

	return true;
}

bool UShopManager::TrySellItem(APlayerController* Player, int32 InvenSlotIdx)
{
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TrySellItem: Player is null"));
		return false;
	}

	UInventoryComponent* InvComp = GetInventoryFromPlayer(Player);
	if (InvComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TrySellItem: Inventory not found"));
		return false;
	}

	const UItemInstance* ItemIns = InvComp->FindItemBySlot(InvenSlotIdx);
	if (ItemIns == nullptr ||
		ItemIns->ItemDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TrySellItem: SlotIdx not valid"));
		return false;
	}

	int32 SellPrice = ItemIns->ItemDef->SellPrice;

	if (InvComp->RemoveItemByDef(ItemIns->ItemDef, 1))
	{
		InvComp->AddBioCoreCount(SellPrice);
		return true;
	}

	return false;
}


bool UShopManager::TryUpgradeStat(APlayerController* Player, const FShopItemData& ItemData)
{
	if (Player == nullptr
		|| ItemData.ItemDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUpgradeStat: invalid args"));
		return false;
	}

	UInventoryComponent* InvComp = GetInventoryFromPlayer(Player);
	if (InvComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUpgradeStat: Inventory not found"));
		return false;
	}

	if (HasEnoughCore(Player, ItemData.ItemDef->BuyPrice) == false)
	{
		UE_LOG(LogTemp, Log, TEXT("TryUpgradeStat: not enough money. Need %d"), ItemData.ItemDef->BuyPrice);
		return false;
	}

	// 실제 Stat 관련 적용 함수 Call
	const bool bApplied = true;

	if (bApplied == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUpgradeStat: apply failed"));
		return false;
	}

	DeductCore(Player, ItemData.ItemDef->BuyPrice);
	return true;
}

bool UShopManager::TryCombineWeapon(APlayerController* Player, const FShopItemData& ItemData)
{
	if (Player == nullptr
		|| ItemData.ItemDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryCombineWeapon: invalid args"));
		return false;
	}

	UInventoryComponent* InvComp = GetInventoryFromPlayer(Player);
	if (InvComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryCombineWeapon: Inventory not found"));
		return false;
	}

	if (ItemData.ItemDef->CraftingMaterials.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryCombineWeapon: Not Crafting Materials"));
		return false;
	}

	for (auto& p : ItemData.ItemDef->CraftingMaterials)
	{
		const UItemDefinition* NeedItemDef = p.Key;
		int32 Amount = p.Value;

		if (NeedItemDef == nullptr ||
			Amount <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("TryCombineWeapon: Crafting Materials is null"));
			return false;
		}

		if (InvComp->CountItem(NeedItemDef) < Amount)
		{
			UE_LOG(LogTemp, Warning, TEXT("TryCombineWeapon: Crafting Materials is Lack"));
			return false;
		}
	}

	for (auto& p : ItemData.ItemDef->CraftingMaterials)
	{
		const UItemDefinition* NeedItemDef = p.Key;
		int32 Amount = p.Value;

		InvComp->RemoveItemByDef(NeedItemDef, Amount);
	}

	GiveItemToInventory(Player, ItemData.ItemDef);

	return true;
}

bool UShopManager::TryEquipWeapon(APlayerController* Player, const FShopItemData& ItemData, EEquipSlot TargetSlot)
{
	if (Player == nullptr
		|| ItemData.ItemDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipWeapon: invalid args"));
		return false;
	}

	UInventoryComponent* InvComp = GetInventoryFromPlayer(Player);
	if (InvComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipWeapon: Inventory not found"));
		return false;
	}

	UWeaponDefinition* WeaponDef = Cast<UWeaponDefinition>(ItemData.ItemDef);
	if (WeaponDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipWeapon: Item is not weapon"));
		return false;
	}

	if (InvComp->FindItemByDef(ItemData.ItemDef) == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipWeapon: Item was not found"));
		return false;
	}

	bool bResult = InvComp->EquipWeaponItem(ItemData.ItemDef, TargetSlot);

	if (bResult == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipWeapon: Equip Failed"));
		return false;
	}

	return true;
}

bool UShopManager::HasEnoughCore(APlayerController* Player, int32 Price)
{
	if (Price <= 0)
		return true;

	if (UInventoryComponent* InvComp = GetInventoryFromPlayer(Player))
	{
		const int32 CoreCount = InvComp->GetBioCoreCount();
		return CoreCount >= Price;
	}

	return false;
}

void UShopManager::DeductCore(APlayerController* Player, int32 Price)
{
	if (Price <= 0)
		return;

	if (UInventoryComponent* InvComp = GetInventoryFromPlayer(Player))
	{
		const bool bOK = InvComp->UseBioCoreCount(Price);
		if (!bOK)
		{
			UE_LOG(LogShop, Warning, TEXT("DeductCore: spend failed (Price:%d)"), Price);
		}
	}
}

void UShopManager::GiveItemToInventory(APlayerController* Player, UItemDefinition* ItemDef)
{
	if (ItemDef == nullptr)
		return;

	if (UInventoryComponent* Inv = GetInventoryFromPlayer(Player))
	{
		Inv->AddItem(ItemDef, 1);
	}
}