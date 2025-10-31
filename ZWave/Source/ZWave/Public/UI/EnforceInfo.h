#pragma once

#include "CoreMinimal.h"
#include "EnforceInfo.generated.h"

USTRUCT(BlueprintType)
struct ZWAVE_API FEnforceInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnforeceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> EnforceValueList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> PriceList;
};
