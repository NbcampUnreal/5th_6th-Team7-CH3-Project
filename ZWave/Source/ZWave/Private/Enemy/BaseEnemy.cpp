// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

#include "Components/SkeletalMeshComponent.h"

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

float ABaseEnemy::GetAttackRange() const
{
	return this->AttackRange;
}

void ABaseEnemy::Attack()
{
	//USkeletalMeshComponent* Mesh = GetMesh();

	if (GetMesh() && AttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(AttackMontage);
		}
	}
}
