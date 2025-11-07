#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ShopTabItem.generated.h"

class UItemCategory;
class UItemSlotButton;
class UVerticalBox;
class UHorizontalBox;
class UPanelItemDesc;

UCLASS()
class ZWAVE_API UShopTabItem : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<UUserWidget> CategoryClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TArray<TObjectPtr<UItemCategory>> Categories;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TArray<UItemSlotButton*> Buttons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UDataTable> CategoryInfoDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item", meta = (BindWidget))
	TObjectPtr<UVerticalBox> ItemCategoryVB;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item", meta = (BindWidget))
	TObjectPtr<UHorizontalBox> MainHB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UPanelItemDesc> ItemDescPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<FName> IgnoreCategories;

public:
	void NativeOnInitialized() override;
	void FindPanel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void OnSelectedButtonChange(UItemSlotButton* NewSelected);
	void OnSelectedButtonChange_Implementation(UItemSlotButton* NewSelected);

	void AddButtons(TArray<UItemSlotButton*> NewButtons);

	bool IsIgnoreRow(FName RowName);
};
