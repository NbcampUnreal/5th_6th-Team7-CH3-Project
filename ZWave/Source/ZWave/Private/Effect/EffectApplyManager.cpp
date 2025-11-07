#include "Effect/EffectApplyManager.h"
#include "Effect/EffectBase.h"
#include "Effect/StaggerEffect.h"
#include "Effect/GainItemEffect.h"
#include "Base/Damagable.h"

UEffectApplyManager::UEffectApplyManager()
{
}

/// <summary>
/// 피격 효과 적용 함수
/// </summary>
/// <param name="Target">적용할 대상</param>
/// <param name="BaseDamage">지속시간 없다면 0</param>
/// <param name="EffectClassArray">효과를 담은 배열</param>
void UEffectApplyManager::ApplyEffect(
	TScriptInterface<IDamagable> Target,
	const TArray<TSubclassOf<UEffectBase>>& EffectClassArray,
	const float& Duration)
{
	if (!Target || EffectClassArray.Num() <= 0)
	{
		return;
	}

	for (const auto& EffectClass : EffectClassArray)
	{
		if (IsValid(Target.GetObject()))
		{
			if (AActor* TargetActor = Cast<AActor>(Target.GetObject()))
			{
				if (UEffectBase* NewEffect = CreateEffect(EffectClass))
				{
					NewEffect->ApplyEffect(TargetActor, Duration);
				}
			}
		}
	}
}

/// <summary>
/// 아이템 효과 적용 함수
/// </summary>
/// <param name="Causer">호출자</param>
/// <param name="EffectClass">적용 효과 클래스</param>
void UEffectApplyManager::ApplyEffect(AActor* Causer, const TArray<TSubclassOf<UEffectBase>>& EffectClassArray, const float& Duration, const int32& ItemIndex)
{
	for (const auto& EffectClass : EffectClassArray)
	{
		if (UEffectBase* NewEffect = CreateEffect(EffectClass))
		{
			if (NewEffect->IsA(UGainItemEffect::StaticClass()))
			{
				NewEffect->ApplyEffect(Causer, ItemIndex);
			}
			else
			{
				NewEffect->ApplyEffect(Causer, Duration);
			}
		}

	}
}

UEffectBase* UEffectApplyManager::CreateEffect(TSubclassOf<UEffectBase> EffectClass)
{
	UEffectBase* NewEffectInstance = nullptr;
	NewEffectInstance = NewObject<UEffectBase>(this, EffectClass);
	return NewEffectInstance;
}

