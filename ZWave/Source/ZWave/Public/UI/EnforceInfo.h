#pragma once

#include "CoreMinimal.h"
#include "Player/TaskPlayer.h"
#include "EnforceInfo.generated.h"

USTRUCT(BlueprintType)
struct ZWAVE_API FEnforceInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText Desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText PreRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText PreName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FText Unit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<float> EnforceValueList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<int32> PriceList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EPlayerShopStat StatType;
};
