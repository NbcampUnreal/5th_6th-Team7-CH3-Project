#pragma once

#include "CoreMinimal.h"

#include "UI/ItemSlotInfo.h"

#include "CommonActivatableWidget.h"
#include "InventoryUI.generated.h"

class UHoveredDescWidgetBase;

UCLASS()
class ZWAVE_API UInventoryUI : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UDataTable> ItemCategoryInfoDataTable;
	TArray<FItemSlotInfo*> InfoList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHoveredDescWidgetBase> Desc;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDescActivated(FName ItemName);
	void OnDescActivated_Implementation(FName ItemName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDescDeactivated();
	void OnDescDeactivated_Implementation();


	FItemSlotInfo* FindItemInfo(FName ItemName);
	void GetDataTable();
};
