// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BombZombie.h"

#include "Base/BaseCharacter.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/EquipComponent.h"


void ABombZombie::BeginPlay()
{
	Super::BeginPlay();
}

void ABombZombie::Die()
{
	//SuisideBomb();

	Super::Die();
}

void ABombZombie::Attacked(AActor* DamageCauser, float Damage)
{
	Super::Attacked(DamageCauser, Damage);
}

void ABombZombie::Suiside()
{

}

