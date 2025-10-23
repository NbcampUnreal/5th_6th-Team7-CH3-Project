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
		CurrentActiveEffects.Add(CreatingEffect);
		CreatingEffect->ApplyEffect(Target, EffectValue);
	}
}

void UEffectApplyManager::RemoveEffectInstance(UEffectBase* EffectToRemove)
{
    if (!EffectToRemove) return;

    CurrentActiveEffects.Remove(EffectToRemove);
	EffectToRemove->MarkAsGarbage();
}


UEffectBase* UEffectApplyManager::CreateEffect(TSubclassOf<UEffectBase> EffectClass)
{
	UEffectBase* FindEffect = nullptr;
	FindEffect = NewObject<UEffectBase>(GetWorld(), EffectClass);
	return FindEffect == nullptr ? nullptr : FindEffect;
}
