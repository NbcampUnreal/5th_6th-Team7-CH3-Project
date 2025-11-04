#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "PanelCraftingDesc.generated.h"

UCLASS()
class ZWAVE_API UPanelCraftingDesc : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Crafting")
	void OnSelectedButtonChanged(UDataTable* ItemCategoryDataTable, UDataTable* IngredientDataTable, FName TargetWeaponName);
};
