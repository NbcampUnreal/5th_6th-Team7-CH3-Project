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

	if (USkeletalMeshComponent* Mesh = OwningCharacter->GetMesh())
	{
		if (Mesh->DoesSocketExist(MuzzleSocketName))
		{
			Start = Mesh->GetSocketLocation(MuzzleSocketName);
			Dir = Mesh->GetForwardVector();
		}
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

	K2_OnFire(Points, Normals, Surfaces);
}
