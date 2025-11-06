// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BombZombie.h"

#include "Base/BaseCharacter.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/EquipComponent.h"




void ABombZombie::Suiside()
{
	this->Attacked(this, 100);
}

