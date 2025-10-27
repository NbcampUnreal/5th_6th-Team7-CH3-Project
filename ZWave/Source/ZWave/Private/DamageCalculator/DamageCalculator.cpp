#include "DamageCalculator/DamageCalculator.h"
#include "Effect/EffectApplyManager.h"
#include "Effect/EffectBase.h"
#include "Base/BaseCharacter.h"
#include "Engine/World.h"

void UDamageCalculator::DamageCalculate(UObject* WorldContextObject, AActor* Causer, AActor* Target, const float& BaseDamage, const float& EffectValue,  TSubclassOf<UEffectBase> EffectClass)
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


	// 수정이 필요할 수 있음
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(Target))
	{
		Character->ApplyDamage(FinalDamage);
	}
}
