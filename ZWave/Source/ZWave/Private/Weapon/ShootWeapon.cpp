// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ShootWeapon.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "DamageCalculator/DamageCalculator.h"
#include "Base/ZWaveGameState.h"
#include "UI/IngameHUD.h"


AShootWeapon::AShootWeapon()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
}

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

	NowAmmo--;

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

	ShootWeaponStatBase = ShootDefinition->ShootWeaponStat;
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
	
	AmmoChangeUIBroadCast();
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
	ReloadUIBroadCast();

	if (ShootWeaponStat.bReloadAll)
	{
		StartReloadAll();
	}
	else
	{
		StartReloadOneBullet();
	}
}

bool AShootWeapon::EquipModing(EModingSlot ModingSlot, UModingInstance* ModeInstance)
{
	if (ModeInstance == nullptr)
		return false;

	const UShootWeaponDefinition* ShootDefinition = Cast<UShootWeaponDefinition>(ModeInstance->GetModeWeaponDef());
	if (ShootDefinition == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's Not ShootWeapon Moding!"));
		return false;
	}

	if (EquipModingMap.Contains(ModingSlot))
	{
		if (EquipModingMap[ModingSlot] != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Need UnEquip Slot"));
			return false;
		}
	}

	EquipModingMap.Add(ModingSlot, ModeInstance);
	EquipModingEffectClassMap.Add(ModingSlot,ModeInstance->GetModeEffectClass());
	ApplyCurrentModing();
	return true;
}

void AShootWeapon::UnEquipModing(EModingSlot ModingSlot)
{
	if (EquipModingMap.Contains(ModingSlot) == false)
	{
		return;
	}
	
	EquipModingMap.Remove(ModingSlot);
	EquipModingEffectClassMap.Remove(ModingSlot);
	ApplyCurrentModing();
}

void AShootWeapon::AddRemainSpareAmmo(float AddingPercent)
{
	if (AddingPercent <= 0)
		return;

	RemainSpareAmmo += (ShootWeaponStat.SpareAmmo * AddingPercent);

	ReloadUIBroadCast();
}

void AShootWeapon::ApplyCurrentModing()
{
	FShootWeaponStats ShootStat = ShootWeaponStatBase;

	for (const auto& Pair : EquipModingMap)
	{
		UModingInstance* ModingIns = Pair.Value;
		if (ModingIns == nullptr)
			continue;

		UShootWeaponDefinition* ShootDef = Cast<UShootWeaponDefinition>(ModingIns->GetModeWeaponDef());
		if (ShootDef == nullptr)
			continue;

		FShootWeaponStats& ModingStat = ShootDef->ShootWeaponStat;
		EWeaponModifier ModifierType = ModingIns->GetModeApplyType();
		
		ApplyStat(ModingStat, ModifierType, ShootStat);
	}

	ShootWeaponStat = ShootStat;

	AmmoChangeUIBroadCast();
}

UIngameHUD* AShootWeapon::GetIngameHud()
{
	if (UWorld* World = GetWorld())
	{
		if (AZWaveGameState* ZGS = Cast<AZWaveGameState>(World->GetGameState()))
		{
			return ZGS->IngameHUD;
		}
	}

	return nullptr;
}

void AShootWeapon::AmmoChangeUIBroadCast()
{
	if (UIngameHUD* nowHud = GetIngameHud())
	{
		nowHud->OnAmmoChanged(NowAmmo, ShootWeaponStat.Magazine, RemainSpareAmmo);
	}
}

void AShootWeapon::ReloadUIBroadCast()
{
	if (UIngameHUD* nowHud = GetIngameHud())
	{
		nowHud->OnReloadWeapon(ShootWeaponStat.ReloadTime);
	}
}

void AShootWeapon::ShootOneBullet(bool IsFPSSight, float SpreadDeg)
{
	if (OwningCharacter == nullptr)
		return;

	AmmoChangeUIBroadCast();

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
			FZWaveDamageEvent DamageEvent;
			DamageEvent.BaseDamage = ShootWeaponStat.AttackPower;
			DamageEvent.Duration = 0.0f;

			TArray<TSubclassOf<UEffectBase>> EffectClasses;
			EquipModingEffectClassMap.GenerateValueArray(EffectClasses);
			DamageEvent.EffectArray = EffectClasses;
			UDamageCalculator::DamageHelper(GetWorld(), TargetActor, OwningCharacter, DamageEvent);

			if (UIngameHUD* nowHud = GetIngameHud())
			{
				nowHud->OnFireWeapon(TargetActor);
			}
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

	AmmoChangeUIBroadCast();

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

	AmmoChangeUIBroadCast();
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

void AShootWeapon::ApplyStat(const FShootWeaponStats& ModingStat, EWeaponModifier ModifierType, FShootWeaponStats& OutStat)
{
	switch (ModifierType)
	{
	case EWeaponModifier::EWM_Add:
	{
		OutStat.AttackPower += ModingStat.AttackPower;
		OutStat.AttackRate	+= ModingStat.AttackRate;
		OutStat.Magazine	+= ModingStat.Magazine;
		OutStat.ReloadTime	+= ModingStat.ReloadTime;
		OutStat.SpareAmmo	+= ModingStat.SpareAmmo;
	}
		break;
	case EWeaponModifier::EWM_Percent:
	{
		OutStat.AttackPower *= (1.0f + ModingStat.AttackPower);
		OutStat.AttackRate	*= (1.0f + ModingStat.AttackRate);
		OutStat.Magazine	*= (1.0f + ModingStat.Magazine);
		OutStat.ReloadTime	*= (1.0f + ModingStat.ReloadTime);
		OutStat.SpareAmmo	*= (1.0f + ModingStat.SpareAmmo);
	}
		break;
	case EWeaponModifier::EWM_Multiple:
	{
		OutStat.AttackPower	*= ModingStat.AttackPower;
		OutStat.AttackRate	*= ModingStat.AttackRate;
		OutStat.Magazine	*= ModingStat.Magazine;
		OutStat.ReloadTime	*= ModingStat.ReloadTime;
		OutStat.SpareAmmo	*= ModingStat.SpareAmmo;
	}
		break;
	}
}
