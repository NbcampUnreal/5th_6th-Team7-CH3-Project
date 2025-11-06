// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemDefinition.h"
#include "ItemDropTable.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FItemDropTable : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	UItemDefinition* ItemDefinition;
};
