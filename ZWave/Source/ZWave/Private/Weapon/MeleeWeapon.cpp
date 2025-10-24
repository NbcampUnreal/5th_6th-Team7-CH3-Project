// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/MeleeWeapon.h"
#include "GameFramework/Character.h"

void AMeleeWeapon::Attack()
{
	if (GetWorld() == nullptr)
		return;

	if (CanAttack() == false ||
		bEquipped == false ||
		OwningCharacter == nullptr)
		return;
}

bool AMeleeWeapon::Init(const UWeaponDefinition* WeaponDefinition)
{
	const UMeleeWeaponDefinition* MeleeDefinition = Cast<UMeleeWeaponDefinition>(WeaponDefinition);
	if (MeleeDefinition == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's Not MeleeWeaponDef!"));
		return false;
	}

	MeleeWeaponStat = MeleeDefinition->MeleeWeaponStat;

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
