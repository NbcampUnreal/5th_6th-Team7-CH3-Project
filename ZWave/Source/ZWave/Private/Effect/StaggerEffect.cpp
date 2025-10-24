#include "Effect/StaggerEffect.h"
#include "Effect/EffectTest.h"
#include "Engine/World.h"
#include "Effect/EffectApplyManager.h"


UStaggerEffect::UStaggerEffect()
{
	StaggerValue = 3;
}

float UStaggerEffect::ApplyEffect(AActor* TargetActor, const float& BaseDamage, float Value)
{
	if (AEffectTest* Test = Cast<AEffectTest>(TargetActor))
	{
		this->Target = Test;

		Test->SetSpeed(Test->GetSpeed() / StaggerValue);

		UE_LOG(LogTemp, Warning, TEXT("Stagger Effect On : %f"), Test->GetSpeed());

		
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
	if (AEffectTest* Test = Cast<AEffectTest>(Target))
	{
		Test->SetSpeed(Test->GetSpeed() * StaggerValue);
		UE_LOG(LogTemp, Warning, TEXT("Stagger Effect Off : %f"), Test->GetSpeed());
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

//µð¹ö±ë¿ë
void UStaggerEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UStaggerEffect '%s' has been successfully collected by GC."), *GetName());
}
