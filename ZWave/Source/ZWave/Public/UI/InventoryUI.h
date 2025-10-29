#pragma once

#include "CoreMinimal.h"

#include "UI/ItemUIInfo.h"

#include "CommonActivatableWidget.h"
#include "InventoryUI.generated.h"

class UHoveredDescWidgetBase;

UCLASS()
class ZWAVE_API UInventoryUI : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UDataTable> ItemUIInfoDataTable;
	TArray<FItemUIInfo*> InfoList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UHoveredDescWidgetBase> Desc;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDescActivated(int32 ID);
	void OnDescActivated_Implementation(int32 ID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDescDeactivated();
	void OnDescDeactivated_Implementation();


	FItemUIInfo* FindItemInfo(int32 ID);
	void GetDataTable();
};
