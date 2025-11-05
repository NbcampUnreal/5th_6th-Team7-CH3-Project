#include "DamageCalculator/DamageCalculator.h"
#include "Effect/EffectApplyManager.h"
#include "Effect/EffectBase.h"
#include "Base/BaseCharacter.h"
#include "Base/Damagable.h"
#include "GenericTeamAgentInterface.h"
#include "Engine/World.h"

void UDamageCalculator::DamageHelper(UObject* WorldContextObject, TScriptInterface<IDamagable> Target, AActor* DamageCauser, FZWaveDamageEvent const& DamageEvent)
{
	IGenericTeamAgentInterface* DamageCauserTeam = nullptr;
	if (DamageCauser->GetInstigator())
	{
		DamageCauserTeam = Cast<IGenericTeamAgentInterface>(DamageCauser->GetInstigator());
	}
	else
	{
		DamageCauserTeam = Cast<IGenericTeamAgentInterface>(DamageCauser->GetOwner());
	}

	IGenericTeamAgentInterface* TargetTeam = Cast<IGenericTeamAgentInterface>(Target.GetObject());

	if (!TargetTeam || !DamageCauserTeam)
	{
		return;
	}

	const FGenericTeamId CauserTeamID = DamageCauserTeam->GetGenericTeamId();
	const FGenericTeamId TargetTeamID = TargetTeam->GetGenericTeamId();

	if (CauserTeamID != TargetTeamID)
	{
		AActor* TargetActor = Cast<AActor>(Target.GetObject());
		if (TargetActor)
		{
			TargetActor->TakeDamage(
				DamageEvent.BaseDamage,
				DamageEvent,
				nullptr,
				DamageCauser
			);
		}
	}
}
