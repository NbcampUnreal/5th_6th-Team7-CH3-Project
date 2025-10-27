// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShootWeapon.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "DamageCalculator/DamageCalculator.h"

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
	
	ShootOneBullet(false,0.0f);

	UE_LOG(LogTemp, Warning, TEXT("Remain Ammo : %d"), NowAmmo);

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
	ShootType = ShootDefinition->ShootType;
	NowAmmo = ShootWeaponStat.Magazine;
	RemainSpareAmmo = ShootWeaponStat.SpareAmmo;
	bReloading = false;

	return true;
}

void AShootWeapon::Equip(ACharacter* NewOwner)
{
	Super::Equip(NewOwner);

	// 장착시 시점은 Owner에서 얻어올 것
	bIsFPSSight = false;
}

void AShootWeapon::Unequip()
{
	Super::Unequip();

	GetWorldTimerManager().ClearTimer(FireTimer);
	GetWorldTimerManager().ClearTimer(ReloadTimer);
	bReloading = false;
	bCanAttack = true;

	OnFireSuccess.Clear();
}

void AShootWeapon::Reload()
{
	if (IsFullMagazine() ||
		bReloading ||
		RemainSpareAmmo <= 0)
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

void AShootWeapon::ShootOneBullet(bool IsFPSSight, float SpreadDeg)
{
	if (OwningCharacter == nullptr)
		return;

	NowAmmo--;

	TArray<FVector> Points, Normals;
	TArray<TEnumAsByte<EPhysicalSurface>> Surfaces;

	FVector Start = OwningCharacter->GetActorLocation();

	if (SkeletalMeshComponent->DoesSocketExist(MuzzleSocketName))
	{
		Start = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);
	}

	FVector CameraAimPoint = GetCameraAimPoint();
	// PlayerController가 없는 경우는 그냥 정면으로
	FVector Shootdir = CameraAimPoint != FVector::ZeroVector ? 
		(CameraAimPoint - Start).GetSafeNormal() : 
		OwningCharacter->GetActorForwardVector();

	// 탄 퍼짐 옵션
	if (SpreadDeg > 0.f)
	{
		const float HalfRad = FMath::DegreesToRadians(SpreadDeg * 0.5f);
		Shootdir = FMath::VRandCone(Shootdir, HalfRad);
	}

	Points.Add(Start);
	Normals.Add(FVector::ZeroVector);
	Surfaces.Add(SurfaceType_Default);

	FVector End = Start + Shootdir * TraceDistance;

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

		AActor* TargetActor = Hit.GetActor();

		if (IsDamagableActor(TargetActor) == true)
		{
			UDamageCalculator::DamageCalculate(
				GetWorld(),
				OwningCharacter,
				TargetActor,
				ShootWeaponStat.AttackPower,
				StaggerValue,
				BaseEffectClasses);
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

	UE_LOG(LogTemp, Warning, TEXT("Remain Ammo : %d"), NowAmmo);

	bCanAttack = true;
	bReloading = false;

	// UI 연동 고려?
}

FVector AShootWeapon::GetCameraAimPoint()
{
	APlayerController* PC = Cast<APlayerController>(OwningCharacter->GetController());
	if (PC == nullptr)
		return FVector::ZeroVector;

	FVector ViewLoc; FRotator ViewRot;
	PC->GetPlayerViewPoint(ViewLoc, ViewRot);

	FVector ViewDir = ViewRot.Vector();
	if (!bIsFPSSight)
	{
		int32 SX, SY;
		PC->GetViewportSize(SX, SY);
		const FVector2D Center(SX * 0.5f, SY * 0.5f);

		FVector WorldOrigin, WorldDir;
		if (PC->DeprojectScreenPositionToWorld(Center.X, Center.Y, WorldOrigin, WorldDir))
		{
			ViewLoc = WorldOrigin;
			ViewDir = WorldDir;
		}
	}

	const float CameraTraceDistance = 15000.f;

	FHitResult AimHit;
	{
		FCollisionQueryParams Params(SCENE_QUERY_STAT(AimTrace), /*bTraceComplex=*/true);
		Params.AddIgnoredActor(OwningCharacter);
		Params.AddIgnoredActor(this);
		const FVector AimEnd = ViewLoc + ViewDir * CameraTraceDistance;
		PC->GetWorld()->LineTraceSingleByChannel(AimHit, ViewLoc, AimEnd, ECC_Visibility, Params);
	}

	return AimHit.bBlockingHit ? AimHit.ImpactPoint : (ViewLoc + ViewDir * CameraTraceDistance);
}
