#pragma once

#include "CoreMinimal.h"

#include "UI/ItemSlotInfo.h"

#include "UI/HoveringDescButtonBase.h"
#include "ItemSlotButton.generated.h"

class UShopTabItem;

UCLASS()
class ZWAVE_API UItemSlotButton : public UHoveringDescButtonBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FItemSlotInfo ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	UShopTabItem* OwnerTab;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mode")
	bool bIsSelected;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item")
	void InitItemSlot(FItemSlotInfo Data, UShopTabItem* Owner);
	void InitItemSlot_Implementation(FItemSlotInfo Data, UShopTabItem* Owner);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item")
	void OnSelectedItem(UCommonButtonBase* Button);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item")
	void OnDeselectedItem();

	FORCEINLINE FItemSlotInfo* GetItemData() { return &ItemData; }
};
