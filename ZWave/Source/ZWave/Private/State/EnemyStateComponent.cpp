// Fill out your copyright notice in the Description page of Project Settings.


#include "State/EnemyStateComponent.h"

#include "Components/SkeletalMeshComponent.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/BaseAIController.h"

// Sets default values for this component's properties
UEnemyStateComponent::UEnemyStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}


void UEnemyStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemyStateComponent::SetState(EEnemyStateType SetType, float Duration)
{
	PreState = CurrentState;
	CurrentState = SetType;
	//UE_LOG(LogTemp, Log, TEXT("StateType: %s -> %s"),
	//	*UEnum::GetValueAsString(PreState), *UEnum::GetValueAsString(CurrentState));

	switch (CurrentState)
	{
	case EEnemyStateType::EST_None:
		OnNone();
		break;
	case EEnemyStateType::EST_Stagger:
		break;
	case EEnemyStateType::EST_Stun:
		OnStun(Duration);
		break;
	case EEnemyStateType::EST_Decoy:
		break;
	case EEnemyStateType::EST_Death:
		OnDeath();
		break;
	default:
		break;
	}
}

void UEnemyStateComponent::OnNone()
{
	ABaseEnemy* MyCharacter = static_cast<ABaseEnemy*>(GetOwner());
	if (MyCharacter == nullptr) return;

	ABaseAIController* AIController = static_cast<ABaseAIController*>(MyCharacter->GetController());
	if (AIController == nullptr) return;

	AIController->SetValueAsBoolToBlackboard(FName(TEXT("IsStunned")), false);
}

void UEnemyStateComponent::OnStun(const float Duration)
{
	ABaseEnemy* MyCharacter = static_cast<ABaseEnemy*>(GetOwner());
	if (MyCharacter == nullptr) return;

	ABaseAIController* AIController = static_cast<ABaseAIController*>(MyCharacter->GetController());
	if (AIController == nullptr) return;

	USkeletalMeshComponent* Mesh = MyCharacter->GetMesh();
	if (Mesh == nullptr) return;

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (AnimInstance == nullptr) return;

	AIController->SetValueAsBoolToBlackboard(FName(TEXT("IsStunned")), true);

	if (Montage_Stun != nullptr)
	{
		AnimInstance->Montage_Play(Montage_Stun, MontageStunPlayRate); //1.5초간 재생

		float delay = (Duration - 1.5f < 0 ? 0 : Duration - 1.5f) + 1.5;
		GetWorld()->GetTimerManager().SetTimer(StateManageHandle, this, &UEnemyStateComponent::RecoverStun, delay, false);
	}
}

void UEnemyStateComponent::RecoverStun()
{
	if (CurrentState == EEnemyStateType::EST_Death)
	{
		return;
	}
	else
	{
		ABaseEnemy* MyCharacter = static_cast<ABaseEnemy*>(GetOwner());
		if (MyCharacter == nullptr) return;

		ABaseAIController* AIController = static_cast<ABaseAIController*>(MyCharacter->GetController());
		if (AIController == nullptr) return;

		USkeletalMeshComponent* Mesh = MyCharacter->GetMesh();
		if (Mesh == nullptr) return;

		UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
		if (AnimInstance == nullptr) return;

		if (Montage_RecoverStun != nullptr)
		{
			AnimInstance->Montage_Play(Montage_RecoverStun, 2.0f);
		}
	}
}

void UEnemyStateComponent::OnDeath()
{
	ABaseEnemy* MyCharacter = static_cast<ABaseEnemy*>(GetOwner());
	if (MyCharacter == nullptr) return;

	ABaseAIController* AIController = static_cast<ABaseAIController*>(MyCharacter->GetController());
	if (AIController == nullptr) return;

	USkeletalMeshComponent* Mesh = MyCharacter->GetMesh();
	if (Mesh == nullptr) return;

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (AnimInstance == nullptr) return;

	if (AIController)
	{
		AIController->StopMovement();
		AIController->StopBehaviorTree();
	}

	// 스턴에서 die로 넘어오는 경우, 죽는 애니메이션 실행x
	if (PreState == EEnemyStateType::EST_Stun)
	{
		// 스턴애니메이션 끝나고 실행(이미 StateManageHanle에 OnRecoverStun이 예약됨)
		GetWorld()->GetTimerManager().ClearTimer(StateManageHandle);
		GetWorld()->GetTimerManager().SetTimer(StateManageHandle, this, &UEnemyStateComponent::ExcuteDestroy, 2, false);
	}
	else
	{
		if (Montage_Die)
		{
			AnimInstance->Montage_Play(Montage_Die);
		}
	}
}

void UEnemyStateComponent::ExcuteDestroy()
{
	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->SetLifeSpan(0.05f);
	}
}

