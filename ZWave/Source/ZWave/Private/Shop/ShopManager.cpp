// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop/ShopManager.h"
#include "Item/InventoryComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "Shop/ShopTableData.h"

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

UShopManager::UShopManager()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> ShopConfigFinder(TEXT("DataTable'/Game/HyunJaeHoon/DataTable/DT_ShopDataTable.DT_ShopDataTable'"));

	if (ShopConfigFinder.Succeeded())
	{
		ShopTable = ShopConfigFinder.Object;
		NameMapSetting();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find DT_WaveConfig!"));
	}
}

bool UShopManager::TryPurchaseItem(APlayerController* Player, const FString& Name)
{
	UItemDefinition* ItemDef = FindItemByDisplayName(Name);

	if (Player == nullptr
		|| ItemDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryPurchaseItem: invalid args"));
		return false;
	}

	if (HasEnoughCore(Player, ItemDef->BuyPrice) == false)
	{
		UE_LOG(LogTemp, Log, TEXT("TryPurchaseItem: not enough money. Need %d"), ItemDef->BuyPrice);
		return false;
	}

	DeductCore(Player, ItemDef->BuyPrice);
	GiveItemToInventory(Player, ItemDef);

	return true;
}

bool UShopManager::TrySellItem(APlayerController* Player, const FString& Name)
{
	UItemDefinition* ItemDef = FindItemByDisplayName(Name);

	if (Player == nullptr ||
		ItemDef == nullptr)
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

	int32 SlotIdx = InvComp->FindItemByDef(ItemDef);
	if (SlotIdx == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("TrySellItem: Item Not Valid"));
		return false;
	}

	int32 SellPrice = ItemDef->SellPrice;

	if (InvComp->RemoveItemBySlot(SlotIdx, 1))
	{
		InvComp->AddBioCoreCount(SellPrice);
		return true;
	}

	return false;
}


bool UShopManager::TryUpgradeStat(APlayerController* Player, const FString& Name)
{
	UItemDefinition* ItemDef = FindItemByDisplayName(Name);

	if (Player == nullptr
		|| ItemDef == nullptr)
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

	if (HasEnoughCore(Player, ItemDef->BuyPrice) == false)
	{
		UE_LOG(LogTemp, Log, TEXT("TryUpgradeStat: not enough money. Need %d"), ItemDef->BuyPrice);
		return false;
	}

	// 실제 Stat 관련 적용 함수 Call
	const bool bApplied = true;

	if (bApplied == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUpgradeStat: apply failed"));
		return false;
	}

	DeductCore(Player, ItemDef->BuyPrice);
	return true;
}

bool UShopManager::TryCombineWeapon(APlayerController* Player, const FString& Name)
{
	UItemDefinition* ItemDef = FindItemByDisplayName(Name);

	if (Player == nullptr
		|| ItemDef == nullptr)
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

	if (ItemDef->CraftingMaterials.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryCombineWeapon: Not Crafting Materials"));
		return false;
	}

	for (auto& p : ItemDef->CraftingMaterials)
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

	for (auto& p : ItemDef->CraftingMaterials)
	{
		const UItemDefinition* NeedItemDef = p.Key;
		int32 Amount = p.Value;

		InvComp->RemoveItemByDef(NeedItemDef, Amount);
	}

	GiveItemToInventory(Player, ItemDef);

	return true;
}

bool UShopManager::TryEquipWeapon(APlayerController* Player, const FString& Name, int32 Slot)
{
	UItemDefinition* ItemDef = FindItemByDisplayName(Name);
	
	if (Player == nullptr
		|| ItemDef == nullptr
		|| Slot < 0
		|| Slot >= static_cast<int32>(EEquipSlot::Third))
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

	const UWeaponDefinition* WeaponDef = Cast<UWeaponDefinition>(ItemDef->Definition);
	if (WeaponDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipWeapon: Item is not weapon"));
		return false;
	}

	if (InvComp->FindItemByDef(ItemDef) == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipWeapon: Item was not found"));
		return false;
	}

	EEquipSlot TargetSlot = static_cast<EEquipSlot>(Slot);
	bool bResult = InvComp->EquipWeaponItem(ItemDef, TargetSlot);

	if (bResult == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipWeapon: Equip Failed"));
		return false;
	}

	return true;
}

bool UShopManager::TryEquipModingToWeapon(APlayerController* Player, const FString& WeaponName, const FString& ModingName, int32 EquipSlot)
{
	if (Player == nullptr ||
		EquipSlot < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipModingToWeapon: invalid args"));
		return false;
	}

	UItemDefinition* WeaponItemDef = FindItemByDisplayName(WeaponName);
	UItemDefinition* ModingItemDef = FindItemByDisplayName(ModingName);

	if (WeaponItemDef == nullptr
		|| ModingItemDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipModingToWeapon: invalid args"));
		return false;
	}

	UInventoryComponent* InvComp = GetInventoryFromPlayer(Player);
	if (InvComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipModingToWeapon: Inventory not found"));
		return false;
	}

	const UWeaponDefinition* WeaponDef = Cast<UWeaponDefinition>(WeaponItemDef->Definition);
	if (WeaponDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipModingToWeapon: Item is not weapon"));
		return false;
	}

	const UWeaponDefinition* ModeDef = Cast<UWeaponDefinition>(ModingItemDef->Definition);
	if (ModeDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipModingToWeapon: Item is not mode"));
		return false;
	}

	bool bResult = InvComp->EquipModingToWeapon(WeaponItemDef, ModingItemDef, EquipSlot);

	if (bResult == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryEquipModingToWeapon: EquipModing Failed"));
		return false;
	}

	return true;
}

bool UShopManager::TryUnequipModingToWeapon(APlayerController* Player, const FString& WeaponName, int32 EquipSlot)
{
	if (Player == nullptr ||
		EquipSlot < 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUnequipModingToWeapon: invalid args"));
		return false;
	}

	UItemDefinition* WeaponItemDef = FindItemByDisplayName(WeaponName);

	if (WeaponItemDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUnequipModingToWeapon: invalid args"));
		return false;
	}

	UInventoryComponent* InvComp = GetInventoryFromPlayer(Player);
	if (InvComp == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUnequipModingToWeapon: Inventory not found"));
		return false;
	}

	const UWeaponDefinition* WeaponDef = Cast<UWeaponDefinition>(WeaponItemDef->Definition);
	if (WeaponDef == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUnequipModingToWeapon: Item is not weapon"));
		return false;
	}

	bool bResult = InvComp->UnequipModingToWeapon(WeaponItemDef, EquipSlot);

	if (bResult == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("TryUnequipModingToWeapon: UnequipModing Failed"));
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

void UShopManager::NameMapSetting()
{
	if (ShopTable == nullptr)
		return;

	DisplayNameToItemMap.Reset();

	for (const auto& Pair : ShopTable->GetRowMap())
	{
		const FShopTableData* Row = reinterpret_cast<const FShopTableData*>(Pair.Value);
		if (Row == nullptr
			|| Row->ItemDef == nullptr)
			continue;

		if (Row->DisplayName.IsEmpty() == false)
		{
			DisplayNameToItemMap.Add(Row->DisplayName, Row->ItemDef);
		}
	}
}

UItemDefinition* UShopManager::FindItemByDisplayName(const FString& Name) const
{
	if (const TObjectPtr<UItemDefinition>* Found = DisplayNameToItemMap.Find(Name))
	{
		return Found->Get();
	}
	return nullptr;
}
