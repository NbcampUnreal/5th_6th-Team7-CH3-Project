#pragma once

#include "CoreMinimal.h"
#include "IngredientInfo.generated.h"

USTRUCT(BlueprintType)
struct ZWAVE_API FPair : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;
};

USTRUCT(BlueprintType)
struct ZWAVE_API FIngredientInfo : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TArray<FPair> IngredientsList;
};
