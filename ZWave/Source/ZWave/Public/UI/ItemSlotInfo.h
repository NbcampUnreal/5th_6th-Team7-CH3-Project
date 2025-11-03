#pragma once

#include "CoreMinimal.h"
#include "ItemSlotInfo.generated.h"

USTRUCT(BlueprintType)
struct ZWAVE_API FItemSlotInfo : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UTexture> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemBuyPrice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemSellPrice;
};
