#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "ShopTabEnforce.generated.h"

class FOnClickPlusButton;
class UEnforceMenu;

UCLASS()
class ZWAVE_API UShopTabEnforce : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enforce")
	TArray<TObjectPtr<UEnforceMenu>> Menus;
	
public:
	FOnClickPlusButton* FindDelegate(FName RowName);
};
