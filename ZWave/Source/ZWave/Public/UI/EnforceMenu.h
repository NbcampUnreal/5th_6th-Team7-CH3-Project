#pragma once

#include "CoreMinimal.h"

#include "UI/EnforceInfo.h"

#include "CommonUserWidget.h"
#include "EnforceMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickPlusButton, float, NewValue);

UCLASS()
class ZWAVE_API UEnforceMenu : public UCommonUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Enforce", meta = (DisplayName = "OnClickPlusButton"))
	FOnClickPlusButton OnClickPlusButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enforce")
	FName EnforceRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enforce")
	FEnforceInfo EnforceData;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Enforce")
	void InitMenu(FEnforceInfo Data);

	UFUNCTION(BlueprintCallable, Category = "Enforce")
	void BroadcastAll(float NewValue);

	FORCEINLINE FOnClickPlusButton& GetDelegate() { return OnClickPlusButton; }
	FORCEINLINE const FName& GetEnforceRowName() { return EnforceRowName; }
};
