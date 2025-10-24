#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "TitleMenuButton.generated.h"

UCLASS()
class ZWAVE_API UTitleMenuButton : public UCommonButtonBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Button", meta = (InlineEditConditionToggle))
	uint8 bOverride_ButtonText : 1;

	UPROPERTY(EditAnywhere, Category = "Button", meta = (editcondition = "bOverride_ButtonText"))
	FText ButtonText;
	
protected:
	virtual void NativePreConstruct() override;

	virtual void UpdateInputActionWidget() override;
	virtual void OnInputMethodChanged(ECommonInputType CurrentInputType) override;


	void RefreshButtonText();

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonText(const FText& InText);

	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonStyle();

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(const FText& InText);
};
