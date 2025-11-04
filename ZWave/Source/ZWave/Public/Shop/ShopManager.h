// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ShopDefinition.h"
#include "Weapon/EquipComponent.h"
#include "ShopManager.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UShopManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void OpenShop(UShopDefinition* ShopDef, APlayerController* Player);

	UFUNCTION(BlueprintCallable)
	bool TryPurchaseItem(APlayerController* Player, const FShopItemData& ItemData);

	UFUNCTION(BlueprintCallable)
	bool TryUpgradeStat(APlayerController* Player, const FShopItemData& ItemData);

	UFUNCTION(BlueprintCallable)
	bool TryCombineWeapon(APlayerController* Player, const FShopItemData& ItemData);

	UFUNCTION(BlueprintCallable)
	bool TryEquipWeapon(APlayerController* Player, const FShopItemData& ItemData, EEquipSlot TargetSlot);

protected:
	bool HasEnoughMoney(APlayerController* Player, int32 Price);
	void DeductMoney(APlayerController* Player, int32 Price);
	void GiveItemToInventory(APlayerController* Player, UItemDefinition* ItemDef);
};
