// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/MeleeWeapon.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "DamageCalculator/DamageCalculator.h"

AMeleeWeapon::AMeleeWeapon()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetGenerateOverlapEvents(false);
	SetRootComponent(SphereCollision);
}

void AMeleeWeapon::Attack()
{
	if (GetWorld() == nullptr)
		return;
	
	if (OverlappedEnemies.Num() == 0)
		return;
	
	if (CanAttack() == false ||
		bEquipped == false ||
		OwningCharacter == nullptr)
		return;

	for (TObjectPtr<AActor> TargetActor : OverlappedEnemies)
	{
		if (IsDamagableActor(TargetActor) == true)
		{
			TArray<TSubclassOf<UEffectBase>> EffectClasses;
			EquipModingEffectClassMap.GenerateValueArray(EffectClasses);

			UDamageCalculator::DamageCalculate(
				GetWorld(),
				OwningCharacter,
				TargetActor,
				MeleeWeaponStat.AttackPower,
				EffectClasses);
		}
	}
}

bool AMeleeWeapon::Init(const UWeaponDefinition* WeaponDefinition)
{
	const UMeleeWeaponDefinition* MeleeDefinition = Cast<UMeleeWeaponDefinition>(WeaponDefinition);
	if (MeleeDefinition == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's Not MeleeWeaponDef!"));
		return false;
	}

	MeleeWeaponStatBase = MeleeDefinition->MeleeWeaponStat;
	MeleeWeaponStat = MeleeDefinition->MeleeWeaponStat;
	SphereCollision->SetSphereRadius(MeleeWeaponStat.Radius);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnSphereBeginOverlap);

	return true;
}

void AMeleeWeapon::Equip(ACharacter* NewOwner)
{
	Super::Equip(NewOwner);
}

void AMeleeWeapon::Unequip()
{
	Super::Unequip();
}

bool AMeleeWeapon::EquipModing(EModingSlot ModingSlot, UModingInstance* ModeInstance)
{
	if (ModeInstance == nullptr)
		return false;

	const UMeleeWeaponDefinition* MeleeDefinition = Cast<UMeleeWeaponDefinition>(ModeInstance->GetModeWeaponDef());
	if (MeleeDefinition == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's Not MeleeWeapon Moding!"));
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

	int32 EquipNum = EquipModingMap.Num();
	if (EquipModingMap.Contains(EModingSlot::EMS_Default))
		EquipNum--;

	if (EquipNum >= MeleeWeaponStat.ModingAllows)
	{
		UE_LOG(LogTemp, Warning, TEXT("Full Moding!"));
		return false;
	}

	EquipModingMap.Add(ModingSlot, ModeInstance);
	EquipModingEffectClassMap.Add(ModingSlot, ModeInstance->GetModeEffectClass());
	ApplyCurrentModing();
	return true;
}

void AMeleeWeapon::UnEquipModing(EModingSlot ModingSlot)
{
	if (EquipModingMap.Contains(ModingSlot) == false)
	{
		return;
	}

	EquipModingMap.Remove(ModingSlot);
	EquipModingEffectClassMap.Remove(ModingSlot);
	ApplyCurrentModing();
}

void AMeleeWeapon::ApplyCurrentModing()
{
	FMeleeWeaponStats MeleeStat = MeleeWeaponStatBase;

	for (const auto& Pair : EquipModingMap)
	{
		UModingInstance* ModingIns = Pair.Value;
		if (ModingIns == nullptr)
			continue;

		UMeleeWeaponDefinition* MeleeDef = Cast<UMeleeWeaponDefinition>(ModingIns->GetModeWeaponDef());
		if (MeleeDef == nullptr)
			continue;

		FMeleeWeaponStats& ModingStat = MeleeDef->MeleeWeaponStat;
		EWeaponModifier ModifierType = ModingIns->GetModeApplyType();

		ApplyStat(ModingStat, ModifierType, MeleeStat);
	}

	MeleeWeaponStat = MeleeStat;
}

void AMeleeWeapon::StartAttack()
{
	SphereCollision->SetGenerateOverlapEvents(true);
}

void AMeleeWeapon::EndAttack()
{
	SphereCollision->SetGenerateOverlapEvents(false);
	Attack();
	OverlappedEnemies.Empty();
	bCanAttack = false;

	GetWorldTimerManager().SetTimer(AttackTimer, [this]()
		{bCanAttack = true; }
	, MeleeWeaponStat.AttackRate, false);
}

void AMeleeWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && 
		(MeleeWeaponStat.bCanOverlapSelf == true || (OtherActor != GetOwner() && OtherActor != OwningCharacter)))
	{
		OverlappedEnemies.AddUnique(OtherActor);
	}
}

void AMeleeWeapon::ApplyStat(const FMeleeWeaponStats& ModingStat, EWeaponModifier ModifierType, FMeleeWeaponStats& OutStat)
{
	switch (ModifierType)
	{
	case EWeaponModifier::EWM_Add:
	{
		OutStat.AttackPower += ModingStat.AttackPower;
		OutStat.AttackRate += ModingStat.AttackRate;
		OutStat.Radius += ModingStat.Radius;
	}
	break;
	case EWeaponModifier::EWM_Percent:
	{
		OutStat.AttackPower *= (1.0f + ModingStat.AttackPower);
		OutStat.AttackRate *= (1.0f + ModingStat.AttackRate);
		OutStat.Radius *= (1.0f + ModingStat.Radius);
	}
	break;
	case EWeaponModifier::EWM_Multiple:
	{
		OutStat.AttackPower *= ModingStat.AttackPower;
		OutStat.AttackRate *= ModingStat.AttackRate;
		OutStat.Radius *= ModingStat.Radius;
	}
	break;
	}
}
