#pragma once

#include "CoreMinimal.h"

#include "UI/ItemSlotInfo.h"

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Desc")
	FName IconMIDValue = "IconTexture";
	TObjectPtr<UMaterialInstanceDynamic> IconMID;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsActivated = false;

public:
	void NativeOnInitialized() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnActivated(const FItemSlotInfo& Info);
	void OnActivated_Implementation(const FItemSlotInfo& Info);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnDeactivated();
	void OnDeactivated_Implementation();
};
