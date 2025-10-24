// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShootWeapon.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

void AShootWeapon::Attack()
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

	if (ShootWeaponStat.bReloadAll == false &&
		bReloading)
	{
		bReloading = false;
		GetWorldTimerManager().ClearTimer(ReloadTimer);
	}

	bCanAttack = false;

	
	ShootOneBullet(false);

	GetWorldTimerManager().SetTimer(FireTimer, [this]() 
		{bCanAttack = true; }
	, ShootWeaponStat.AttackRate, false);
}

bool AShootWeapon::Init(const UWeaponDefinition* WeaponDefinition)
{
	const UShootWeaponDefinition* ShootDefinition = Cast<UShootWeaponDefinition>(WeaponDefinition);
	if (ShootDefinition == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's Not ShootWeaponDef!"));
		return false;
	}

	ShootWeaponStat = ShootDefinition->ShootWeaponStat;
	MuzzleSocketName = ShootDefinition->MuzzleSocketName;
	TraceDistance = ShootDefinition->TraceDistance;
	NowAmmo = ShootWeaponStat.Magazine;
	RemainSpareAmmo = ShootWeaponStat.SpareAmmo;
	
	bReloading = false;

	return true;
}

void AShootWeapon::Equip(ACharacter* NewOwner)
{
	Super::Equip(NewOwner);
}

void AShootWeapon::Unequip()
{
	Super::Unequip();

	GetWorldTimerManager().ClearTimer(FireTimer);
	GetWorldTimerManager().ClearTimer(ReloadTimer);
	bReloading = false;
	bCanAttack = true;
}

void AShootWeapon::Reload()
{
	if (IsFullMagazine() ||
		bReloading ||
		RemainSpareAmmo)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShootWeapon Can't Reload!"));
		return;
	}

	bReloading = true;

	if (ShootWeaponStat.bReloadAll)
	{
		StartReloadAll();
	}
	else
	{
		StartReloadOneBullet();
	}
}

void AShootWeapon::ShootOneBullet(bool IsFPSSight)
{
	if (OwningCharacter == nullptr)
		return;

	NowAmmo--;

	TArray<FVector> Points, Normals;
	TArray<TEnumAsByte<EPhysicalSurface>> Surfaces;

	FVector Start = OwningCharacter->GetActorLocation();
	FVector Dir = OwningCharacter->GetActorForwardVector();

	if (SkeletalMeshComponent->DoesSocketExist(MuzzleSocketName))
	{
		Start = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);
	}

	Points.Add(Start);
	Normals.Add(FVector::ZeroVector);
	Surfaces.Add(SurfaceType_Default);

	FVector End = Start + Dir * TraceDistance;

	FHitResult Hit;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ShootWeaponTrace), /*bTraceComplex=*/true);
	Params.AddIgnoredActor(OwningCharacter);
	Params.AddIgnoredActor(this);

	bool bHit = OwningCharacter->GetWorld()->LineTraceSingleByChannel(
		Hit, Start, End, ECC_Visibility, Params);

	// TODO : 현 시점에 '관통 총알' 모딩은 고려하지 않음
	if (bHit)
	{
		Points.Add(Hit.ImpactPoint);
		Normals.Add(Hit.ImpactNormal);
		Surfaces.Add(UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get()));

		if (AActor* TargetActor = Hit.GetActor())
		{
			// Damage 계산 함수 요청
		}
	}
	else
	{
		Points.Add(End);
		Normals.Add(FVector::ZeroVector);
		Surfaces.Add(SurfaceType_Default);
	}

	K2_OnFire(Points, Normals, Surfaces,bHit);
}

void AShootWeapon::StartReloadOneBullet()
{
	if (IsFullMagazine() || RemainSpareAmmo <= 0)
	{
		return;
	}

	GetWorldTimerManager().SetTimer(ReloadTimer, this, &AShootWeapon::ReloadOneBullet,
		ShootWeaponStat.ReloadTime, false, ShootWeaponStat.ReloadTime);
}

void AShootWeapon::ReloadOneBullet()
{
	if (IsFullMagazine() || RemainSpareAmmo <= 0)
	{
		StopReloadOneBullet();
		return;
	}

	RemainSpareAmmo--;
	NowAmmo++;

	// UI 연동 고려?

	GetWorldTimerManager().SetTimer(ReloadTimer,this, &AShootWeapon::ReloadOneBullet,
	ShootWeaponStat.ReloadTime, false, ShootWeaponStat.ReloadTime);
}

void AShootWeapon::StopReloadOneBullet()
{
	GetWorldTimerManager().ClearTimer(ReloadTimer);
	bReloading = false;
}

void AShootWeapon::StartReloadAll()
{
	if (IsFullMagazine() || RemainSpareAmmo <= 0)
		return;

	bCanAttack = false;
	bReloading = true;

	GetWorldTimerManager().SetTimer(ReloadTimer, this, &AShootWeapon::ReloadAll,
		ShootWeaponStat.ReloadTime, false, ShootWeaponStat.ReloadTime);
}

void AShootWeapon::ReloadAll()
{
	if (IsFullMagazine() || RemainSpareAmmo <= 0)
		return;

	const int NeedBullet = ShootWeaponStat.Magazine - NowAmmo;
	const int Move = CalcTransferBullet(NeedBullet);

	if (Move <= 0)
		return;

	RemainSpareAmmo -= Move;
	NowAmmo += Move;

	bCanAttack = true;
	bReloading = false;

	// UI 연동 고려?
}