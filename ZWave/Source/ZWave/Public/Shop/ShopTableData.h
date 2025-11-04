// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // FTableRowBase 정의 포함
#include "Item/ItemDefinition.h"
#include "ShopTableData.generated.h"

USTRUCT(BlueprintType)
struct FShopTableData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop")
    TObjectPtr<UItemDefinition> ItemDef = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop")
    FString DisplayName;
};
