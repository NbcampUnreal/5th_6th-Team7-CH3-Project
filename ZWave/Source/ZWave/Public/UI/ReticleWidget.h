#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "ReticleWidget.generated.h"

UCLASS()
class ZWAVE_API UReticleWidget : public UCommonUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnReticleActivated();

	UFUNCTION(BlueprintImplementableEvent)
	void OnFireWeapon(AActor* HitActor);

	UFUNCTION(BlueprintImplementableEvent)
	void OnReloadWeapon(float ReloadTime);
};
