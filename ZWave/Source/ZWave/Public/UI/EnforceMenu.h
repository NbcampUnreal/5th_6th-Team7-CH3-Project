#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enforce")
	FName EnforceRowName;

public:
	UFUNCTION(BlueprintCallable, Category = "Enforce")
	void BroadcastAll(float NewValue);

	FORCEINLINE FOnClickPlusButton& GetDelegate() { return OnClickPlusButton; }
	FORCEINLINE const FName& GetEnforceRowName() { return EnforceRowName; }
};
