// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_RecoverStun.h"

#include "Enemy/BaseEnemy.h"
#include "Enemy/BaseAIController.h"

#include "State/EnemyStateComponent.h"


void UAnimNotify_RecoverStun::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp == nullptr) return;

	ABaseEnemy* Owner = static_cast<ABaseEnemy*>(MeshComp->GetOwner());
	if (Owner == nullptr) return;

	Owner->StateComp->SetState(EEnemyStateType::EST_None);
}
