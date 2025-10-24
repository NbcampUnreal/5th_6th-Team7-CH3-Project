#include "DamageCalculator/DamageCalculator.h"
#include "Effect/EffectApplyManager.h"
#include "Engine/World.h"

void UDamageCalculator::DamageCalculate(UObject* WorldContextObject, AActor* Causer, AActor* Target, const float& BaseDamage, float EffectValue,  TSubclassOf<class UEffectBase> EffectClass)
{
	float FinalDamage = BaseDamage;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (UEffectApplyManager* EffectApplyManager = World->GetSubsystem<UEffectApplyManager>())
		{
			EffectApplyManager->ApplyEffect(Target, BaseDamage, EffectValue, EffectClass);
		}
	}


	// 여기서 데미지 적용
	UE_LOG(LogTemp, Warning, TEXT("FinalDamage : %f"), FinalDamage);
}
