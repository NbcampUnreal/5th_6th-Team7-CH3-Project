// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Engine/DataTable.h"
#include "Weapon/EquipComponent.h"
#include "ShopManager.generated.h"

class UItemDefinition;

/**
 * 
 */
UCLASS()
class ZWAVE_API UShopManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UShopManager();

	UFUNCTION(BlueprintCallable)
	bool TryPurchaseItem(APlayerController* Player, const FString& Name);

	UFUNCTION(BlueprintCallable)
	bool TrySellItem(APlayerController* Player, const FString& Name);

	UFUNCTION(BlueprintCallable)
	bool TryUpgradeStat(APlayerController* Player, const FString& Name);

	UFUNCTION(BlueprintCallable)
	bool TryCombineWeapon(APlayerController* Player, const FString& Name);

	UFUNCTION(BlueprintCallable)
	bool TryEquipWeapon(APlayerController* Player, const FString& Name, int32 Slot);

	UFUNCTION(BlueprintCallable)
	bool TryEquipModingToWeapon(APlayerController* Player, const FString& WeaponName, const FString& ModingName, int32 EquipSlot);

	UFUNCTION(BlueprintCallable)
	UItemDefinition* FindItemByDisplayName(const FString& Name) const;

protected:
	bool HasEnoughCore(APlayerController* Player, int32 Price);
	void DeductCore(APlayerController* Player, int32 Price);
	void GiveItemToInventory(APlayerController* Player, UItemDefinition* ItemDef);

	void NameMapSetting();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Shop Data")
	TObjectPtr<UDataTable> ShopTable = nullptr;

	UPROPERTY(VisibleAnywhere)
	TMap<FString, TObjectPtr<UItemDefinition>> DisplayNameToItemMap;
};
