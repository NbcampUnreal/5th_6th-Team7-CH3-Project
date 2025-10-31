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
	TArray<TSubclassOf<UEffectBase>>& EffectClassArray 
)
{
	float FinalDamage = BaseDamage;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (UEffectApplyManager* EffectApplyManager = World->GetSubsystem<UEffectApplyManager>())
		{
			EffectApplyManager->ApplyEffect(Target, BaseDamage, EffectClassArray);
		}
	}

	// 여기서 데미지 적용
	UE_LOG(LogTemp, Warning, TEXT("FinalDamage : %f"), FinalDamage);
	Target->Attacked(Causer, FinalDamage);
}

void UDamageCalculator::DamageHelper(UObject* WorldContextObject, AActor* Target, AActor* DamageCauser, FZWaveDamageEvent const& DamageEvent)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull))
	{
		if (UEffectApplyManager* EffectApplyManager = World->GetSubsystem<UEffectApplyManager>())
		{
			EffectApplyManager->ApplyEffect(Target, DamageEvent.BaseDamage, DamageEvent.EffectArray);
		}
	}

	// 이 부분에서 같은 팀이면 TakeDamage 호출 되지 않게 수정
	//Target->TakeDamage(DamageEvent.BaseDamage, DamageEvent, Target->GetInstigatorController(), DamageCauser);

	if (ABaseCharacter* Player = Cast<ABaseCharacter>(Target))
	{
		Player->Attacked(Player, DamageEvent.BaseDamage);
	}
}
