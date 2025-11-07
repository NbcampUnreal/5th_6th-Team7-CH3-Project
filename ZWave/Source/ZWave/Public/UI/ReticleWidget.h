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
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnReticleActivated();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnFireWeapon(AActor* HitActor);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnReloadWeapon(float ReloadTime);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnAmmoChanged(float CurrentAmmo, float MaxAmmo);
};
