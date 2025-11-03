#pragma once

#include "CoreMinimal.h"
#include "UI/TitleMenuButton.h"
#include "HoveringDescButtonBase.generated.h"

class UInventoryUI;

UCLASS()
class ZWAVE_API UHoveringDescButtonBase : public UTitleMenuButton
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
	TObjectPtr<UInventoryUI> Inventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName TargetItemName;

public:
	void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHoveringIn();
	void OnHoveringIn_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHoveringOut();
	void OnHoveringOut_Implementation();
};
