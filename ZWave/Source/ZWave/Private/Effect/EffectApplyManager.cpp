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

//void UEffectApplyManager::RemoveAllEffect()
//{
//	for(int i = 0; i < CurrentApplyEffects.Num(); ++i)
//	{
//		CurrentApplyEffects[i]->RemoveEffect();
//		CurrentApplyEffects[i] = nullptr;
//	}
//
//	CurrentApplyEffects.Empty();
//}
//
//void UEffectApplyManager::RemoveEffect(UEffectBase* RemoveClass)
//{
//	for (int i = 0; i < CurrentApplyEffects.Num(); ++i)
//	{
//		if (CurrentApplyEffects[i] == RemoveClass)
//		{
//			CurrentApplyEffects[i] = nullptr;
//			CurrentApplyEffects.RemoveAt(i);
//			break;
//		}
//	}
//}

UEffectBase* UEffectApplyManager::CreateEffect(TSubclassOf<UEffectBase> EffectClass)
{
	UEffectBase* FindEffect = nullptr;
	FindEffect = NewObject<UEffectBase>(GetWorld(), EffectClass);

	CurrentApplyEffects.Add(FindEffect);

	return FindEffect == nullptr ? nullptr : FindEffect;
}
