#pragma once

#include "CoreMinimal.h"

#include "UI/ItemCategoryInfo.h"

#include "CommonUserWidget.h"
#include "ItemCategory.generated.h"

class UItemSlotButton;
class UUniformGridPanel;
class UShopTabItem;

UCLASS()
class ZWAVE_API UItemCategory : public UCommonUserWidget
{
	GENERATED_BODY()
	
protected:
	const int32 MODES_IN_ROW = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<UUserWidget> ItemSlotButtonClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TArray<UItemSlotButton*> ItemSlotButtons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FItemCategoryInfo CategoryData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item", meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemSlotButtonGrid;

	UShopTabItem* ParentTab;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void InitCategory(FItemCategoryInfo Data, UShopTabItem* Parent);
	void InitCategory_Implementation(FItemCategoryInfo Data, UShopTabItem* Parent);
};
