#include "Effect/EffectApplyManager.h"
#include "Effect/EffectBase.h"
#include "Effect/StaggerEffect.h"
#include "Base/Damagable.h"

UEffectApplyManager::UEffectApplyManager()
{
}

float UEffectApplyManager::ApplyEffect(
	TScriptInterface<IDamagable> Target,
	const float& BaseDamage, 
	float EffectValue, 
	const TArray<TSubclassOf<UEffectBase>>& EffectClassArray)
{
	float ApplyEffectDmg = BaseDamage;

	for (const auto& EffectClass : EffectClassArray)
	{
		if (IsValid(Target.GetObject()))
		{
			if (AActor* TargetActor = Cast<AActor>(Target.GetObject()))
			{
				if (UEffectBase* NewEffect = CreateEffect(EffectClass))
				{
					ApplyEffectDmg += NewEffect->ApplyEffect(TargetActor, BaseDamage, EffectValue);
				}
			}
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

