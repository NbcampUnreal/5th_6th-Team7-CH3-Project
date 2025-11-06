// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

#include "State/EnemyStateComponent.h"
#include "Enemy/BaseAIController.h"
#include "Prop/Turret.h"
#include "AoE/AoEActor.h"


ABaseEnemy::ABaseEnemy()
{
	TeamID = 2;

	StateComp = CreateDefaultSubobject<UEnemyStateComponent>(TEXT("UEnemyStateComponent"));
	//MyComp = CreateDefaultSubobject<UMyActorComponent>(TEXT("UMyActorComponent"));
}

void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
}

UAnimMontage* ABaseEnemy::GetAttackedMontage(EHitDir Direction)
{
	switch (Direction)
	{
	case EHitDir::Front:
		return FrontHitMontage;
	case EHitDir::Back:
		return BackHitMontage;
	case EHitDir::Left:
		return LeftHitMontage;
	case EHitDir::Right:
		return RightHitMontage;
	default:
		return nullptr;
	}
}

void ABaseEnemy::Attacked(AActor* DamageCauser, float Damage)
{
	Super::Attacked(DamageCauser, Damage);

	if (Health <= 0.f) return;
	
	CheckPriorityLv(DamageCauser);
}

void ABaseEnemy::PlayHitAnimMontage(AActor* DamageCauser)
{
	if (StateComp->GetCurrentState() == EEnemyStateType::EST_Stun) {
		UE_LOG(LogTemp, Display, TEXT("hit when stun"));
		return;
	}

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
			AnimInstance->Montage_Play(PlayAnim);
		}
	}
}

void ABaseEnemy::CheckPriorityLv(AActor* DamageCauser)
{
	//if (bCanEditAttackPriority == false) return;
	ABaseAIController* AIController = static_cast<ABaseAIController*>(GetController());
	if (AIController == nullptr) return;

	int32 CurPriorityLv = AIController->GetValueAsIntFromBlackboard(FName(TEXT("CurPriorityLv")));
	if (DamageCauser->IsA(ABaseCharacter::StaticClass()) && CurPriorityLv < 3 && MaxPriorityLv >= 2)
	{
		AIController->SetValueAsIntToBlackboard(FName(TEXT("CurPriorityLv")), 2);
		SetNewTarget(DamageCauser);

		PlayHitAnimMontage(DamageCauser);
	}
	else if (DamageCauser->IsA(ATurret::StaticClass()))
	{
		if (CurPriorityLv < 2 && MaxPriorityLv >= 1)
		{
			AIController->SetValueAsIntToBlackboard(FName(TEXT("CurPriorityLv")), 1);
			SetNewTarget(DamageCauser);
		}
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
	else if (DamageCauser->IsA(AAoEActor::StaticClass()))
	{
		AIController->SetValueAsIntToBlackboard(FName(TEXT("CurPriorityLv")), 3);
		SetNewTarget(DamageCauser);
	}
}

void ABaseEnemy::SetNewTarget(AActor* DamageCauser)
{
	ABaseAIController* AIController = static_cast<ABaseAIController*>(GetController());
	if (AIController == nullptr) return;

	if(DamageCauser != nullptr)
	{
		FVector SecondaryTargetLocation = DamageCauser->GetActorLocation();
		FVector AttackLocation = AIController->GetAttackLocation(SecondaryTargetLocation);

		AIController->SetValueAsObjectToBlackboard(FName(TEXT("SecondaryTarget")), DamageCauser);
		AIController->SetValueAsVectorToBlackboard(FName(TEXT("AttackLocation")), AttackLocation);
		AIController->SetValueAsBoolToBlackboard(FName(TEXT("IsAggroed")), true);
	}
	else // 외부에서 타겟을 해제할수 있도록 해준다.
	{
		AIController->ClearValueFromBlackboard(FName(TEXT("SecondaryTarget")));
		AIController->ClearValueFromBlackboard(FName(TEXT("AttackLocation")));
		AIController->SetValueAsIntToBlackboard(FName(TEXT("CurPriorityLv")), 0);

		AIController->SetValueAsBoolToBlackboard(FName(TEXT("IsAggroed")), false);
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
	StateComp->SetState(EEnemyStateType::EST_Death);
}


int32 ABaseEnemy::GetMaxPriorityLv() const
{
	return this->MaxPriorityLv;
}

float ABaseEnemy::GetAttackRange() const
{
	return this->AttackRange;
}

float ABaseEnemy::GetSightRange() const
{
	return this->SightRange;
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
