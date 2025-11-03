#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ShopTabEnforce.generated.h"

class FOnClickPlusButton;
class UEnforceMenu;
class UCommonHierarchicalScrollBox;

UCLASS()
class ZWAVE_API UShopTabEnforce : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enforce")
	TSubclassOf<UUserWidget> MenuClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enforce")
	TArray<TObjectPtr<UEnforceMenu>> Menus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enforce")
	TObjectPtr<UDataTable> EnforceInfoDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enforce", meta = (BindWidget))
	TObjectPtr<UCommonHierarchicalScrollBox> MenuScrollBox;
	
public:
	void NativeOnInitialized() override;

	FOnClickPlusButton* FindDelegate(FName RowName);
};
