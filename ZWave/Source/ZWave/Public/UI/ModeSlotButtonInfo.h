#pragma once

#include "CoreMinimal.h"
#include "ModeSlotButtonInfo.generated.h"

USTRUCT(BlueprintType)
struct ZWAVE_API FModeSlotButtonInfo : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	TObjectPtr<UTexture> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	int32 Price;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	FText Desc;
};
