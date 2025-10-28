#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Weapon/ShootWeaponDefinition.h"
#include "IngameHUD.generated.h"

class UReticleWidget;

UCLASS()
class ZWAVE_API UIngameHUD : public UCommonUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UOverlay> Background;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UReticleWidget> CurrentReticle;

public:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnHealthChange(float CurrentHealth, float TargetHealth);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnGunChanged(EShootType SlotIdx);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnAmmoChanged(int32 CurrentAmmo, int32 MaxAmmo, int32 TotalAmmo);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnWaveChanged(int32 NewWave, int32 MaxWave);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnEnemyCountChanged(int32 CurrentEnemyCount, int32 MaxEnemyCount);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnBreakTimeStart();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnBattleStart();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnTimeUpdated();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnChangedWaveMode(bool bIsBattle);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnFireWeapon(AActor* HitActor);
	void OnFireWeapon_Implementation(AActor* HitActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnReloadWeapon(float ReloadTime);
	void OnReloadWeapon_Implementation(float ReloadTime);
};
