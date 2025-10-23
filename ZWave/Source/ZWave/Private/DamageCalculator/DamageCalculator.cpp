#include "DamageCalculator/DamageCalculator.h"
#include "Effect/EffectApplyManager.h"
#include "Engine/World.h"

float UDamageCalculator::DamageCalculate(UObject* WorldContextObject, AActor* Target, float EffectValue, TSubclassOf<class UEffectBase> EffectClass)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (UEffectApplyManager* EffectApplyManager = World->GetSubsystem<UEffectApplyManager>())
		{
			EffectApplyManager->ApplyEffect(Target, EffectValue, EffectClass);
		}
	}

	return 0.0f;
}
