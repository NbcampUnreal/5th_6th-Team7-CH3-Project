#pragma once

#include "CoreMinimal.h"

#include "UI/ModeSlotButtonInfo.h"

#include "CommonUserWidget.h"
#include "PanelModeDesc.generated.h"

class UModeSlotButton;

UCLASS()
class ZWAVE_API UPanelModeDesc : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mode")
	void OnSelectedButtonChanged(UModeSlotButton* Selected);
};
