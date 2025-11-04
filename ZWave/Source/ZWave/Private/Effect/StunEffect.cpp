// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/StunEffect.h"
#include "State/StateComponent.h"
#include "Enemy/BaseAIController.h"

UStunEffect::UStunEffect()
{
}

void UStunEffect::ApplyEffect(AActor* TargetActor, const float& Duration)
{
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(TargetActor))
	{
		Target = Enemy;
		static_cast<ABaseAIController*>(Enemy->GetController())->SetValueAsBoolToBlackboard(FName(TEXT("IsStunned")), true);
		if (UStateComponent* StateComp = Enemy->FindComponentByClass<UStateComponent>())
		{
			StateComp->SetState(EStateType::EST_Stun);
			UE_LOG(LogTemp, Log, TEXT("EffectType: %s"),
				*UEnum::GetValueAsString(StateComp->GetCurrentState()));
		}
	}

	SetStunTimer(5);
	return;

	/*if (!FMath::IsNearlyZero(Duration))
	{
		SetStunTimer(Duration);
	}
	else
	{
		RemoveEffect();
	}*/
}

void UStunEffect::RemoveEffect()
{
	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			World->GetTimerManager().ClearTimer(StunTimer);
		}
	}

	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(Target))
	{
		if (UStateComponent* StateComp = Enemy->FindComponentByClass<UStateComponent>())
		{
			static_cast<ABaseAIController*>(Enemy->GetController())->SetValueAsBoolToBlackboard(FName(TEXT("IsStunned")), false);
			StateComp->SetState(EStateType::EST_None);
		}
	}

	Super::RemoveEffect();
}

void UStunEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UStunEffect has been successfully collected by GC."));
}

void UStunEffect::SetStunTimer(const float& Duration)
{
	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			World->GetTimerManager().SetTimer(
				StunTimer,
				this,
				&UStunEffect::RemoveEffect,
				Duration,
				false
			);
		}
	}
}
