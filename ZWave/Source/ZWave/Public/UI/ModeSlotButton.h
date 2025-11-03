#pragma once

#include "CoreMinimal.h"

#include "UI/ModeSlotButtonInfo.h"

#include "CommonButtonBase.h"
#include "ModeSlotButton.generated.h"

class UPanelModeSlot;

UCLASS()
class ZWAVE_API UModeSlotButton : public UCommonButtonBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	FModeSlotButtonInfo ModeData;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mode")
	TObjectPtr<UPanelModeSlot> ParentPanel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mode")
	bool bIsSelected;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mode")
	void InitModeSlot(FModeSlotButtonInfo Data, UPanelModeSlot* Parent);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mode")
	void OnSelectedMode(UCommonButtonBase* Button);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Mode")
	void OnDeselectedMode();

	FORCEINLINE FModeSlotButtonInfo* GetModeData() { return &ModeData; }
};
