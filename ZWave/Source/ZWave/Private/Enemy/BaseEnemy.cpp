// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

ABaseEnemy::ABaseEnemy()
{
	
}

void ABaseEnemy::Attacked(AActor* DamageCauser, float Damage)
{
	Super::Attacked(DamageCauser, Damage);
}

void ABaseEnemy::ApplyDamage(float Damage, bool CheckArmor)
{
	Super::ApplyDamage(Damage, CheckArmor);
}

void ABaseEnemy::Die()
{
	Super::Die();
}

bool ABaseEnemy::GetCanEditAttackPriority() const
{
	return this->bCanEditAttackPriority;
}

void ABaseEnemy::Attack()
{
}
