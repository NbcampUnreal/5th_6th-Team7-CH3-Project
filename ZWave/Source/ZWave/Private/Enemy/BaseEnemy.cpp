// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABaseEnemy::ABaseEnemy()
{
	
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	//Health = MaxHealth;
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

void ABaseEnemy::SetMoveSpeed(float MoveSpeed)
{
	if (MoveSpeed < 0) return;

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}
}

float ABaseEnemy::GetMoveSpeed()
{
	if (GetCharacterMovement())
	{
		return GetCharacterMovement()->MaxWalkSpeed;
	}

	return 0;
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
