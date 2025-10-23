#include "Effect/EffectApplyManager.h"
#include "Effect/EffectBase.h"
#include "Effect/StaggerEffect.h"
#include "Mode/ModeDefinition.h"

UEffectApplyManager::UEffectApplyManager()
{
}

void UEffectApplyManager::ApplyEffect(AActor* Target, float EffectValue, TSubclassOf<UEffectBase> EffectClass)
{
	if (UEffectBase* CreatingEffect = CreateEffect(EffectClass))
	{
		CreatingEffect->ApplyEffect(Target, EffectValue);
	}
}

UEffectBase* UEffectApplyManager::CreateEffect(TSubclassOf<UEffectBase> EffectClass)
{
	UEffectBase* FindEffect = nullptr;
	FindEffect = NewObject<UEffectBase>(GetWorld(), EffectClass);
	return FindEffect == nullptr ? nullptr : FindEffect;
}
