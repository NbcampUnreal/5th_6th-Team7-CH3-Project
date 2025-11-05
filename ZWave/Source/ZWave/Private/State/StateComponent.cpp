// Fill out your copyright notice in the Description page of Project Settings.


#include "State/StateComponent.h"

#include "Components/SkeletalMeshComponent.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/BaseAIController.h"

// Sets default values for this component's properties
UStateComponent::UStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	CurrentState = EStateType::EST_None;
}


// Called when the game starts
void UStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

//
//void UStateComponent::BeginPlay()
//{
//	Super::BeginPlay();
//
//	/*OwningCharacter = static_cast<ABaseEnemy*>(GetOwner());
//	if (OwningCharacter == nullptr) return;
//
//	AIController = static_cast<ABaseAIController*>(OwningCharacter->GetController());
//	if (AIController == nullptr) return;
//
//	Mesh = OwningCharacter->GetMesh();
//	if (Mesh == nullptr) return;
//
//	AnimInstance = Mesh->GetAnimInstance();
//	if (AnimInstance == nullptr) return;*/
//}
//
//void UStateComponent::SetState(EStateType SetType, float Duration)
//{
//	//CurrentState = SetType;
//	//switch (GetCurrentState())
//	//{
//	//case EStateType::EST_None:
//	//	OnNone();
//	//	break;
//	//case EStateType::EST_Stagger:
//	//	break;
//	//case EStateType::EST_Stun:
//	//	OnStun(Duration);
//	//	break;
//	//case EStateType::EST_Decoy:
//	//	break;
//	//default:
//	//	break;
//	//}
//}
//
//void UStateComponent::OnNone()
//{
//	//AIController->SetValueAsBoolToBlackboard(FName(TEXT("IsStunned")), false);
//}
//
//void UStateComponent::OnStun(const float Duration)
//{
//	//AIController->SetValueAsBoolToBlackboard(FName(TEXT("IsStunned")), true);
//
//	//if (Montage_Stun != nullptr)
//	//{
//	//	AnimInstance->Montage_Play(Montage_Stun, 1.5f); //1.5초간 재생
//
//	//	float delay = (Duration - 1.5f < 0 ? 0 : Duration - 1.5f) + 1.5;
//	//	GetWorld()->GetTimerManager().SetTimer(StateManageHandle, this, &UStateComponent::OnRecoverStun, delay, false);
//	//}
//}

void UStateComponent::SetState(EStateType SetType)
{
	CurrentState = SetType;
}


