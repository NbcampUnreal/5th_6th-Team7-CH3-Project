// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
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
	bool TryPurchaseItem(APlayerController* Player, UItemDefinition* ItemDef);

	UFUNCTION(BlueprintCallable)
	bool TrySellItem(APlayerController* Player, int32 InvenSlotIdx);

	UFUNCTION(BlueprintCallable)
	bool TryUpgradeStat(APlayerController* Player, const UItemDefinition* ItemDef);

	UFUNCTION(BlueprintCallable)
	bool TryCombineWeapon(APlayerController* Player, UItemDefinition* ItemDef);

	UFUNCTION(BlueprintCallable)
	bool TryEquipWeapon(APlayerController* Player, const UItemDefinition* ItemDef, EEquipSlot TargetSlot);

protected:
	bool HasEnoughCore(APlayerController* Player, int32 Price);
	void DeductCore(APlayerController* Player, int32 Price);
	void GiveItemToInventory(APlayerController* Player, UItemDefinition* ItemDef);

};
