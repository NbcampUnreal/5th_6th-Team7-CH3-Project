#include "Effect/SpeedEffect.h"

USpeedEffect::USpeedEffect()
{
	SpeedMultiplier = 0.3f;
	bIsDebuff = false;
}

void USpeedEffect::ApplyEffect(AActor* TargetActor, const float& BaseDamage)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(TargetActor))
	{
		Target = Character;
		// 이 부분 임시 작성
		if (bIsDebuff)
		{
			// 이속 감소 효과 적용
		}
		else
		{
			// 이속 증가 효과 적용
		}
	}
}

void USpeedEffect::RemoveEffect()
{
	// 이속 증가/감소 효과 제거 로직 작성




	Super::RemoveEffect();
}

void USpeedEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("USpeedEffect has been successfully collected by GC."));
}
