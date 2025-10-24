// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/ShootWeaponDefinition.h"
#include "ShootWeapon.generated.h"

/**
 *
 */
UCLASS()
class ZWAVE_API AShootWeapon : public AWeaponBase
{
	GENERATED_BODY()
public:
	virtual void Attack() override;

	virtual bool Init(const UWeaponDefinition* WeaponDefinition) override;

	virtual void Equip(ACharacter* NewOwner) override;

	virtual void Unequip() override;

	virtual void Reload();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int GetNowAmmo() { return NowAmmo; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int GetRemainSpareAmmo() { return RemainSpareAmmo; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int GetMagazine() { return ShootWeaponStat.Magazine; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsNeedReload() { return NowAmmo <= 0; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsFullMagazine() { return NowAmmo == ShootWeaponStat.Magazine; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int CalcTransferBullet(int Need) const { return FMath::Min(Need, RemainSpareAmmo); }

public:
	// BP 이펙트 처리용
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon", meta = (DisplayName = "OnFire"))
	void K2_OnFire(
		const TArray<FVector>& ImpactPositions,
		const TArray<FVector>& ImpactNormals,
		const TArray<TEnumAsByte<EPhysicalSurface>>& SurfaceTypes,
		bool bHit
	);

protected:
	// TODO : 차후 시점 변경이 있다면 Delegate를 구독할 필요 있음
	// 샷건 고려시, 총알 각도 계산 변경 필요 (폴리싱 기간 작업 예정)
	void ShootOneBullet(bool IsFPSSight);

	void StartReloadOneBullet();
	void ReloadOneBullet();
	void StopReloadOneBullet();

	void StartReloadAll();
	void ReloadAll();

protected:
	FShootWeaponStats ShootWeaponStat;
	FName MuzzleSocketName;
	float TraceDistance;

	int NowAmmo;
	int RemainSpareAmmo;
	bool bReloading;

	FTimerHandle FireTimer;
	FTimerHandle ReloadTimer;
};
