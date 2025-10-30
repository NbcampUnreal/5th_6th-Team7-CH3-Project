#include "Effect/StaggerEffect.h"
#include "Engine/World.h"
#include "Effect/EffectApplyManager.h"
#include "Player/TaskPlayer.h"
#include "Enemy/BaseEnemy.h"


UStaggerEffect::UStaggerEffect()
{
	StaggerSpeedMultiplier = 2;
}

void UStaggerEffect::ApplyEffect(AActor* TargetActor, const float& BaseDamage)
{
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(TargetActor))
	{
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
	// 경직 효과 제거 (몬스터 코드 추가 후)
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(Target))
	{
		Enemy->SetMoveSpeed(Enemy->GetMoveSpeed() * StaggerSpeedMultiplier);
		UE_LOG(LogTemp, Warning, TEXT("Stagger Effect DeActive! EnemySpeed : %f"), Enemy->GetMoveSpeed());
		Target = nullptr;
	}

	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			World->GetTimerManager().ClearTimer(Handle);
		}
	}

	MarkAsGarbage(); //가비지 마킹
}


//디버깅용 코드
void UStaggerEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UStaggerEffect '%s' has been successfully collected by GC."), *GetName());
}
