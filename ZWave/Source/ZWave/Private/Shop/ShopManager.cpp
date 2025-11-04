// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop/ShopManager.h"
#include "Item/InventoryComponent.h"

void UShopManager::OpenShop(UShopDefinition* ShopDef, APlayerController* Player)
{

}

bool UShopManager::TryPurchaseItem(APlayerController* Player, const FShopItemData& ItemData)
{
	return false;
}

bool UShopManager::TryUpgradeStat(APlayerController* Player, const FShopItemData& ItemData)
{
	return false;
}

bool UShopManager::TryCombineWeapon(APlayerController* Player, const FShopItemData& ItemData)
{
	return false;
}

bool UShopManager::TryEquipWeapon(APlayerController* Player, const FShopItemData& ItemData, EEquipSlot TargetSlot)
{
	return false;
}

bool UShopManager::HasEnoughMoney(APlayerController* Player, int32 Price)
{
	return false;
}

void UShopManager::DeductMoney(APlayerController* Player, int32 Price)
{

}

void UShopManager::GiveItemToInventory(APlayerController* Player, UItemDefinition* ItemDef)
{

}