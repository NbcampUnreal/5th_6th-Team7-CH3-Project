#pragma once

#include "CoreMinimal.h"
#include "UI/InventoryUI.h"
#include "ShopUI.generated.h"

UCLASS()
class ZWAVE_API UShopUI : public UInventoryUI
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TArray<TObjectPtr<UCommonActivatableWidget>> TabList;

public:
	void NativeOnInitialized() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnChangeTab(int32 TargetTabIdx);
	void OnChangeTab_Implementation(int32 TargetTabIdx);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnUpdateBioCoin(float DummyValue);
};
