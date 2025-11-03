// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangedEnemy.h"
#include "Enemy/BaseAIController.h"

ARangedEnemy::ARangedEnemy()
{
}

void ARangedEnemy::Attack()
{
	if (GetMesh() && AttackMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(
				AttackMontage,
				2.f,
				EMontagePlayReturnType::MontageLength,
				0.f,
				true
			);
		}
	}
}
