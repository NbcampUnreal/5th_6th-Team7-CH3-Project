#include "Effect/StaggerEffect.h"
#include "Engine/World.h"
#include "Effect/EffectApplyManager.h"
#include "Player/TaskPlayer.h"


UStaggerEffect::UStaggerEffect()
{
	StaggerValue = 3;
}

float UStaggerEffect::ApplyEffect(AActor* TargetActor, const float& BaseDamage, const float& Value)
{
	// 몬스터 이동 속도 제어 여기 내부 작성(몬스터 코드 추가 후)
	if (ATaskPlayer* Test = Cast<ATaskPlayer>(TargetActor))
	{
		this->Target = Test;
		
		if (UObject* Outer = GetOuter())
		{
			if (UWorld* World = Outer->GetWorld())
			{
				World->GetTimerManager().SetTimer(
					Handle,
					this,
					&UStaggerEffect::RemoveEffect,
					Value,
					false
				);
			}
			
		}
	}

	return BaseDamage;
}

void UStaggerEffect::RemoveEffect()
{
	// 경직 효과 제거 (몬스터 코드 추가 후)
	if (ATaskPlayer* Test = Cast<ATaskPlayer>(Target))
	{
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
