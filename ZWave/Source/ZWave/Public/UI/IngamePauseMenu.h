#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "IngamePauseMenu.generated.h"

UCLASS()
class ZWAVE_API UIngamePauseMenu : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnGameOver(bool bIsPlayerDie);
};
