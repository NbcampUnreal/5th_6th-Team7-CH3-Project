#pragma once

#include "CoreMinimal.h"

#include "UI/ItemUIInfo.h"

#include "CommonUserWidget.h"
#include "HoveredDescWidgetBase.generated.h"

class UTextBlock;
class UImage;

UCLASS()
class ZWAVE_API UHoveredDescWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TitleText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DescText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsActivated = false;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnActivated(const FItemUIInfo& Info);
	void OnActivated_Implementation(const FItemUIInfo& Info);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDeactivated();
	void OnDeactivated_Implementation();
};
