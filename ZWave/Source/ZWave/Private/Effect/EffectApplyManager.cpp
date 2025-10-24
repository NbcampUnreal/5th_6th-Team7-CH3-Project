#include "Effect/EffectApplyManager.h"
#include "Effect/EffectBase.h"
#include "Effect/StaggerEffect.h"
#include "Mode/ModeDefinition.h"

UEffectApplyManager::UEffectApplyManager()
{
}

float UEffectApplyManager::ApplyEffect(AActor* Target, const float& BaseDamage, float EffectValue, TSubclassOf<UEffectBase> EffectClass)
{
	float ApplyEffectDmg = BaseDamage;
	if (UEffectBase* NewEffect = CreateEffect(EffectClass))
	{
		if (IsValid(Target))
		{
			ApplyEffectDmg = NewEffect->ApplyEffect(Target, BaseDamage, EffectValue);
		}
	}

	return ApplyEffectDmg;
}

UEffectBase* UEffectApplyManager::CreateEffect(TSubclassOf<UEffectBase> EffectClass)
{
	UEffectBase* NewEffectInstance = nullptr;
	NewEffectInstance = NewObject<UEffectBase>(this, EffectClass);
	return NewEffectInstance;
}
