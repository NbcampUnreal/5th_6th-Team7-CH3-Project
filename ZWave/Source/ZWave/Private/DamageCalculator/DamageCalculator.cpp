#include "DamageCalculator/DamageCalculator.h"
#include "Effect/EffectApplyManager.h"
#include "Effect/EffectBase.h"
#include "Base/BaseCharacter.h"
#include "Base/Damagable.h"
#include "Engine/World.h"

void UDamageCalculator::DamageCalculate(
	UObject* WorldContextObject, 
	AActor* Causer, 
	TScriptInterface<IDamagable> Target, 
	const float& BaseDamage, 
	const float& EffectValue,  
	TArray<TSubclassOf<UEffectBase>>& EffectClassArray 
)
{
	float FinalDamage = BaseDamage;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (UEffectApplyManager* EffectApplyManager = World->GetSubsystem<UEffectApplyManager>())
		{
			FinalDamage += EffectApplyManager->ApplyEffect(Target, BaseDamage, EffectValue, EffectClassArray);
		}
	}

	// 여기서 데미지 적용
	UE_LOG(LogTemp, Warning, TEXT("FinalDamage : %f"), FinalDamage);
	Target->ApplyDamage(FinalDamage);
}
