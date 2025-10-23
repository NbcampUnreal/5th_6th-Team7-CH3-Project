#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "IngameHUD.generated.h"

UCLASS()
class ZWAVE_API UIngameHUD : public UCommonUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> Background;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChange(float CurrentHealth, float TargetHealth);

	UFUNCTION(BlueprintImplementableEvent)
	void OnGunChanged(int32 SlotIdx);

	UFUNCTION(BlueprintImplementableEvent)
	void OnAmmoChanged(int32 CurrentAmmo, int32 MaxAmmo, int32 TotalAmmo);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWaveChanged(int32 NewWave, int32 MaxWave);

	UFUNCTION(BlueprintImplementableEvent)
	void OnEnemyCountChanged(int32 CurrentEenemyCount, int32 MaxEnemyCount);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnTimeUpdated(FTimerHandle Timer);
};
