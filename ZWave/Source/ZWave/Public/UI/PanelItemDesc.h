#pragma once

#include "CoreMinimal.h"

#include "UI/ItemSlotInfo.h"

#include "CommonUserWidget.h"
#include "PanelItemDesc.generated.h"

class UItemSlotButton;

UCLASS()
class ZWAVE_API UPanelItemDesc : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mode")
	void OnSelectedButtonChanged(UItemSlotButton* Selected);
};
