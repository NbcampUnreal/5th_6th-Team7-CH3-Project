#pragma once

#include "CoreMinimal.h"

#include "UI/IngredientInfo.h"
#include "UI/ItemCategoryInfo.h"

#include "CommonActivatableWidget.h"
#include "ShopTabCrafting.generated.h"

UCLASS()
class ZWAVE_API UShopTabCrafting : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TObjectPtr<UDataTable> IngredientInfoTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Crafting")
	TObjectPtr<UDataTable> ItemCategoryInfoTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crafting")
	FItemCategoryInfo WeaponInfo;
};
