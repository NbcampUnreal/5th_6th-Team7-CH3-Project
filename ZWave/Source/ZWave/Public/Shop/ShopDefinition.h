// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Item/ItemDefinition.h"
#include "ShopDefinition.generated.h"

UENUM(BlueprintType)
enum class EUpStatType : uint8
{
	EST_None,
	EST_Health,
	EST_Speed,
};

USTRUCT(BlueprintType)
struct FShopItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TObjectPtr<UItemDefinition> ItemDef;

	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	int32 Stock;

	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	EUpStatType UpgradeStatType;

	bool IsValidItem() const
	{
		return ItemDef != nullptr;
	}

	int32 GetItemPrice() const
	{
		if (IsValidItem() == false)
			return -1;

		return ItemDef->SellPrice;
	}
};

UCLASS(BlueprintType)
class UShopDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TArray<FShopItemData> Items;
};