// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BehaviorTree.h"	
#include "BrainComponent.h"

#include "Enemy/BaseAIController.h"

ABaseEnemy::ABaseEnemy()
{
	
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

UAnimMontage* ABaseEnemy::GetAttackedMontage(EHitDir Direction)
{
	return nullptr;
}

void ABaseEnemy::Attacked(AActor* DamageCauser, float Damage)
{
	Super::Attacked(DamageCauser, Damage);
	
	if (GetMesh())
	{
		FVector SelfLocation = GetActorLocation();
		FVector CauserLocation = DamageCauser->GetActorLocation();
		FVector ToCauser = (CauserLocation - SelfLocation).GetSafeNormal();
		FVector Forward = GetActorForwardVector();
		FVector Right = GetActorRightVector();

		float ForwardDot = FVector::DotProduct(Forward, ToCauser);
		float RightDot = FVector::DotProduct(Right, ToCauser);

		FString HitDirection;
		UAnimMontage* PlayAnim = nullptr;

		if (ForwardDot > 0.707f)
		{
			PlayAnim = GetAttackedMontage(EHitDir::Front);
		}
		else if (ForwardDot < -0.707f)
		{
			PlayAnim = GetAttackedMontage(EHitDir::Back);
		}
		else if (RightDot > 0)
		{
			PlayAnim = GetAttackedMontage(EHitDir::Right);
		}
		else
		{
			PlayAnim = GetAttackedMontage(EHitDir::Left);
		}

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{

		}
	}
}

void ABaseEnemy::ApplyDamage(float Damage, bool CheckArmor)
{
	if (Health <= 0.f) return;

	Super::ApplyDamage(Damage, CheckArmor);
}

void ABaseEnemy::Die()
{
	//Super::Die();
	if (ABaseAIController* AIController = static_cast<ABaseAIController*>(GetController()))
	{
		AIController->StopMovement();
		AIController->StopBehaviorTree();
	}

	if (GetMesh() && DieMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DieMontage);
		}
	}
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
