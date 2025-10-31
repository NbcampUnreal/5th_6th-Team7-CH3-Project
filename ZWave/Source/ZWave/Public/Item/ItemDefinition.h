// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDefinition.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    EIT_None        UMETA(DisplayName = "None"),
    EIT_Weapon      UMETA(DisplayName = "Weapon"),
    EIT_Mode        UMETA(DisplayName = "Mode"),
    EIT_Projectile  UMETA(DisplayName = "Projectile"),
    EIT_Material    UMETA(DisplayName = "Material")
};

UCLASS(Abstract, BlueprintType)
class ZWAVE_API UItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId(FPrimaryAssetType("Item"), GetFName());
    }

    //아이템 이름
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Definition")
    FText ItemName;

    //아이템 타입
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Definition")
    EItemType ItemType;

    //아이템 설명
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Definition")
    FText ItemDescription;

    //최대 스택 개수, 무기,모드 아이템은 건드릴 필요없음
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Definition")
    int32 MaxStack;

    //아이템 아이콘
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Definition")
    TSoftObjectPtr<UTexture2D> ItemIcon;

    // 상점 구매 가격 (0이면 구매 불가)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Definition")
    int32 BuyPrice = 0;

    // 상점 판매 가격 (0이면 판매 불가)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Definition")
    int32 SellPrice = 0;

    //(무기전용) 무기 재료
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
    TMap<UItemDefinition*, int32> CraftingMaterials;

    //(무기전용) 무기 모드 수용량
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Stats")
    int32 ModSlotLimit;

};
