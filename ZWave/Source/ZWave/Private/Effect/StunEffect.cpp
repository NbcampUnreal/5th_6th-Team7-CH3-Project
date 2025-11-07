// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/StunEffect.h"

#include "Enemy/BaseAIController.h"
#include "State/EnemyStateComponent.h"


UStunEffect::UStunEffect()
{
}

void UStunEffect::ApplyEffect(AActor* TargetActor, const float& Duration)
{
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(TargetActor))
	{
		Target = Enemy;
		Enemy->StateComp->SetState(EEnemyStateType::EST_Stun, Duration);
		RemoveEffect();
	}
}

void UStunEffect::RemoveEffect()
{
	Super::RemoveEffect();
}

void UStunEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UStunEffect has been successfully collected by GC."));
}

