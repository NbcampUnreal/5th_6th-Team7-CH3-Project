// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ShopDefinition.h"
#include "Weapon/EquipComponent.h"
#include "ShopManager.generated.h"

UENUM(BlueprintType)
enum class EShopFunctionResult : uint8
{
	ESFR_Success,			// 성공
	ESFR_TargetNull,		// 대상이 없음 (Player Or Component)
	ESFR_BioCoreLack,		// 바이오코어 부족
	ESFR_ResourceLack,		// 조합 자원 부족
	ESFR_StockLack,			// 재고 부족
	ESFR_CompletedUpgrade,	// 이미 완료된 업그레이드
};

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
