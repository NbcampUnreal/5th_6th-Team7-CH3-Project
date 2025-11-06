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

	OwningCharacter = static_cast<ABaseEnemy*>(GetOwner());
	if (OwningCharacter == nullptr) return;

	AIController = static_cast<ABaseAIController*>(OwningCharacter->GetController());
	if (AIController == nullptr) return;

	Mesh = OwningCharacter->GetMesh();
	if (Mesh == nullptr) return;

	AnimInstance = Mesh->GetAnimInstance();
	if (AnimInstance == nullptr) return;
}

void UEnemyStateComponent::SetState(EEnemyStateType SetType, float Duration)
{
	CurrentState = SetType;
	switch (GetCurrentState())
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
	default:
		break;
	}
}

void UEnemyStateComponent::OnNone()
{
	AIController->SetValueAsBoolToBlackboard(FName(TEXT("IsStunned")), false);
}

void UEnemyStateComponent::OnStun(const float Duration)
{
	AIController->SetValueAsBoolToBlackboard(FName(TEXT("IsStunned")), true);

	if (Montage_Stun != nullptr)
	{
		AnimInstance->Montage_Play(Montage_Stun, 1.5f); //1.5초간 재생

		float delay = (Duration - 1.5f < 0 ? 0 : Duration - 1.5f) + 1.5;
		GetWorld()->GetTimerManager().SetTimer(StateManageHandle, this, &UEnemyStateComponent::OnRecoverStun, delay, false);
	}
}

void UEnemyStateComponent::OnRecoverStun()
{
	if (Montage_RecoverStun != nullptr)
	{
		AnimInstance->Montage_Play(Montage_RecoverStun, 2.0f);
	}
}

