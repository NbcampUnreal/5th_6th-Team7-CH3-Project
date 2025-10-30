// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/ShootWeaponDefinition.h"
#include "ShootWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponFireSuccess);

/**
 *
 */
UCLASS()
class ZWAVE_API AShootWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AShootWeapon();

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
	FORCEINLINE bool IsReload() { return bReloading; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsFullMagazine() { return NowAmmo == ShootWeaponStat.Magazine; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int CalcTransferBullet(int Need) const { return FMath::Min(Need, RemainSpareAmmo); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EShootType GetShootType() const { return ShootType; }

public:
	// BP 이펙트 처리용
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon", meta = (DisplayName = "OnFire"))
	void K2_OnFire(
		const TArray<FVector>& ImpactPositions,
		const TArray<FVector>& ImpactNormals,
		const TArray<TEnumAsByte<EPhysicalSurface>>& SurfaceTypes,
		bool bHit
	);

public:
	UPROPERTY()
	FOnWeaponFireSuccess OnFireSuccess;

protected:
	class UIngameHUD* GetIngameHud();
	void AmmoChangeUIBroadCast();
	void ReloadUIBroadCast();

protected:
	void ShootOneBullet(bool IsFPSSight, float SpreadDeg);

	void StartReloadOneBullet();
	void ReloadOneBullet();
	void StopReloadOneBullet();

	void StartReloadAll();
	void ReloadAll();

	FVector GetCameraAimPoint();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	FShootWeaponStats ShootWeaponStat;
	FName MuzzleSocketName;
	float TraceDistance;

	int NowAmmo;
	int RemainSpareAmmo;
	bool bReloading;

	FTimerHandle FireTimer;
	FTimerHandle ReloadTimer;

	EShootType ShootType;

	bool bIsFPSSight;
};
