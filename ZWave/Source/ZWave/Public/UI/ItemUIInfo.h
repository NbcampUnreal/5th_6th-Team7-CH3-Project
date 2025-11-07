#pragma once

#include "CoreMinimal.h"
#include "ItemUIInfo.generated.h"

USTRUCT(BlueprintType)
struct ZWAVE_API FItemUIInfo : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Desc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UMaterialInterface> Icon;
};
