#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "PanelModeSlot.generated.h"

class UModeSlotButton;
class UUniformGridPanel;
class UPanelModeDesc;

UCLASS()
class ZWAVE_API UPanelModeSlot : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	const int32 MODES_IN_ROW = 3;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	TSubclassOf<UUserWidget> ModeSlotClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mode")
	TArray<TObjectPtr<UModeSlotButton>> ModeSlotList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	TObjectPtr<UDataTable> ModeSlotInfoDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mode", meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ModeSlotGrid;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mode")
	TObjectPtr<UPanelModeDesc> ModeDescPanel;

public:
	void NativeOnInitialized() override;
	void FindPanel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mode")
	void OnSelectedButtonChanged(UModeSlotButton* NewSelected);
	void OnSelectedButtonChanged_Implementation(UModeSlotButton* NewSelected);
};
