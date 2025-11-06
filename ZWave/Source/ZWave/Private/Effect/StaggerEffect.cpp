#include "Effect/StaggerEffect.h"
#include "Engine/World.h"
#include "Effect/EffectApplyManager.h"
#include "Player/TaskPlayer.h"
#include "Enemy/BaseEnemy.h"

#include "State/EnemyStateComponent.h"




UStaggerEffect::UStaggerEffect()
{
	StaggerSpeedMultiplier = 2;
}

void UStaggerEffect::ApplyEffect(AActor* TargetActor, const float& Duration)
{
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(TargetActor))
	{
		if (Enemy->StateComp->GetCurrentState() == EEnemyStateType::EST_Stagger) {
			return;
		}
		else
		{
			Enemy->StateComp->SetState(EEnemyStateType::EST_Stagger);
		}
		/*if (UStateComponent* StateComp = Enemy->FindComponentByClass<UStateComponent>())
		{
			if (StateComp->GetCurrentState() == EStateType::EST_Stagger)
			{
				return;
			}
			else
			{
				StateComp->SetState(EStateType::EST_Stagger);
			}
		}*/


		this->Target = Enemy;
		Enemy->SetMoveSpeed(Enemy->GetMoveSpeed() / StaggerSpeedMultiplier);
		UE_LOG(LogTemp, Warning, TEXT("Stagger Effect Active! EnemySpeed : %f"), Enemy->GetMoveSpeed());
		
		if (UObject* Outer = GetOuter())
		{
			if (UWorld* World = Outer->GetWorld())
			{
				World->GetTimerManager().SetTimer(
					Handle,
					this,
					&UStaggerEffect::RemoveEffect,
					StaggerTime,
					false
				);
			}
		}
	}
}

void UStaggerEffect::RemoveEffect()
{
	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			World->GetTimerManager().ClearTimer(Handle);
		}
	}

	//경직 효과 제거 (몬스터 코드 추가 후)
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(Target))
	{
		Enemy->StateComp->SetState(EEnemyStateType::EST_None);
		/*if (UStateComponent* StateComp = Enemy->FindComponentByClass<UStateComponent>())
		{
			StateComp->SetState(EStateType::EST_None);
		}*/


		Enemy->SetMoveSpeed(Enemy->GetMoveSpeed() * StaggerSpeedMultiplier);
		UE_LOG(LogTemp, Warning, TEXT("Stagger Effect DeActive! EnemySpeed : %f"), Enemy->GetMoveSpeed());
	}

	Super::RemoveEffect();
}


//디버깅용 코드
void UStaggerEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UStaggerEffect '%s' has been successfully collected by GC."), *GetName());
}
