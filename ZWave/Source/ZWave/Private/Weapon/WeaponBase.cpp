// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponBase.h"
#include "GameFramework/Character.h"

AWeaponBase::AWeaponBase()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
}

void AWeaponBase::Equip(ACharacter* NewOwner)
{
	OwningCharacter = NewOwner;
}

void AWeaponBase::Unequip()
{
	OwningCharacter = nullptr;
}