#include "Effect/EffectApplyManager.h"
#include "Effect/EffectBase.h"
#include "Effect/StaggerEffect.h"
#include "Mode/ModeDefinition.h"

UEffectApplyManager::UEffectApplyManager()
{
}

void UEffectApplyManager::ApplyEffect(AActor* Target)
{
	if (UEffectBase* CreatingEffect = CreateEffect(EEffectType::Stagger))
	{
		CreatingEffect->ApplyEffect(Target, 0.3f);
	}
}

void UEffectApplyManager::RemoveAllEffect()
{
	for(int i = 0; i < CurrentApplyEffects.Num(); ++i)
	{
		CurrentApplyEffects[i]->RemoveEffect();
		CurrentApplyEffects[i] = nullptr;
	}

	CurrentApplyEffects.Empty();
}

void UEffectApplyManager::RemoveEffect(UEffectBase* RemoveClass)
{
	for (int i = 0; i < CurrentApplyEffects.Num(); ++i)
	{
		if (CurrentApplyEffects[i] == RemoveClass)
		{
			CurrentApplyEffects[i] = nullptr;
			CurrentApplyEffects.RemoveAt(i);
		}
	}
}

UEffectBase* UEffectApplyManager::CreateEffect(const EEffectType& Type)
{
	UEffectBase* FindEffect = nullptr;
	FindEffect = NewObject<UEffectBase>(GetWorld(), UStaggerEffect::StaticClass());

	CurrentApplyEffects.Add(FindEffect);

	return FindEffect == nullptr ? nullptr : FindEffect;
}
