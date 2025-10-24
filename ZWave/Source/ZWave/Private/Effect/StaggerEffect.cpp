#include "Effect/StaggerEffect.h"
#include "Engine/World.h"
#include "Effect/EffectApplyManager.h"
#include "Player/TaskPlayer.h"


UStaggerEffect::UStaggerEffect()
{
	StaggerValue = 3;
}

float UStaggerEffect::ApplyEffect(AActor* TargetActor, const float& BaseDamage, float Value)
{
	if (ATaskPlayer* Test = Cast<ATaskPlayer>(TargetActor))
	{
		this->Target = Test;

		/*Test->SetSpeed(Test->GetSpeed() / StaggerValue);

		UE_LOG(LogTemp, Warning, TEXT("Stagger Effect On : %f"), Test->GetSpeed());*/

		
		if (UObject* Outer = GetOuter())
		{
			if (UWorld* World = Outer->GetWorld())
			{
				World->GetTimerManager().SetTimer(
					Handle,
					this,
					&UStaggerEffect::RemoveEffect,
					Value,
					false
				);
			}
			
		}
	}

	return BaseDamage;
}

void UStaggerEffect::RemoveEffect()
{
	if (ATaskPlayer* Test = Cast<ATaskPlayer>(Target))
	{
		/*Test->SetSpeed(Test->GetSpeed() * StaggerValue);
		UE_LOG(LogTemp, Warning, TEXT("Stagger Effect Off : %f"), Test->GetSpeed());*/
		Target = nullptr;
	}


	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			World->GetTimerManager().ClearTimer(Handle);
		}
	}
	MarkAsGarbage();
}


void UStaggerEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UStaggerEffect '%s' has been successfully collected by GC."), *GetName());
}
