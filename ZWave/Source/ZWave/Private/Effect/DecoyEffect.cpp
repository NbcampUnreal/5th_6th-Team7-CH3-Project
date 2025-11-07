// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DecoyEffect.h"
#include "Kismet/GameplayStatics.h"
#include "State/EnemyStateComponent.h"


UDecoyEffect::UDecoyEffect()
{
}

void UDecoyEffect::ApplyEffect(AActor* TargetActor, const float& Duration)
{

	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(TargetActor))
	{
		Target = Enemy;
		Enemy->StateComp->SetState(EEnemyStateType::EST_Decoy);
	}


	if (!FMath::IsNearlyZero(Duration))
	{
		SetDecoyTimer(Duration);
	}
	else
	{
		RemoveEffect();
	}
}

void UDecoyEffect::RemoveEffect()
{
	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			World->GetTimerManager().ClearTimer(DecoyTimer);

			if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(Target))
			{
				Enemy->StateComp->SetState(EEnemyStateType::EST_None);
				Enemy->SetNewTarget(nullptr);
			}
		}
	}

	Super::RemoveEffect();
}

void UDecoyEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UDecoyEffect has been successfully collected by GC."));
}

void UDecoyEffect::SetDecoyTimer(const float& Duration)
{
	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			World->GetTimerManager().SetTimer(
				DecoyTimer,
				this,
				&UDecoyEffect::RemoveEffect,
				Duration,
				false
			);
		}
	}
}
