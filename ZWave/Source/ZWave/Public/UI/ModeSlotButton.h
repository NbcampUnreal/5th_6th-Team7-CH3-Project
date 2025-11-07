#pragma once

#include "CoreMinimal.h"

#include "UI/ItemSlotInfo.h"

#include "CommonButtonBase.h"
#include "ModeSlotButton.generated.h"

class UPanelModeSlot;

UCLASS()
class ZWAVE_API UModeSlotButton : public UCommonButtonBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	FItemSlotInfo ModeData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mode")
	TObjectPtr<UPanelModeSlot> ParentPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mode")
	bool bIsSelected;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mode")
	void InitModeSlot(FItemSlotInfo Data, UPanelModeSlot* Parent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mode")
	void OnSelectedMode(UCommonButtonBase* Button);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mode")
	void OnDeselectedMode();

	FORCEINLINE FItemSlotInfo* GetModeData() { return &ModeData; }
};
