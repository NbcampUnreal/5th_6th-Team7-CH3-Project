#pragma once

#include "CoreMinimal.h"
#include "ItemSlotInfo.h"
#include "ItemCategoryInfo.generated.h"

USTRUCT(BlueprintType)
struct ZWAVE_API FItemCategoryInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName CategoryName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FItemSlotInfo> ItemSlotInfoList;
};
