// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShotGunWeapon.h"

void AShotGunWeapon::Attack()
{
	if (GetWorld() == nullptr)
		return;

	if (CanAttack() == false ||
		bEquipped == false ||
		OwningCharacter == nullptr ||
		IsNeedReload())
		return;

	if (ShootWeaponStat.bReloadAll &&
		bReloading)
	{
		return;
	}

	if (OnFireSuccess.IsBound())
	{
		OnFireSuccess.Broadcast();
	}

	if (ShootWeaponStat.bReloadAll == false &&
		bReloading)
	{
		bReloading = false;
		GetWorldTimerManager().ClearTimer(ReloadTimer);
	}

	bCanAttack = false;

	NowAmmo--;

	for(int i = 0; i < ShotGunBulletCount;i++)
		ShootOneBullet(false, ShotGunBulletDegree);

	UE_LOG(LogTemp, Warning, TEXT("Remain Ammo : %d"), NowAmmo);


	GetWorldTimerManager().SetTimer(FireTimer, [this]()
		{bCanAttack = true; }
	, ShootWeaponStat.AttackRate, false);
}
