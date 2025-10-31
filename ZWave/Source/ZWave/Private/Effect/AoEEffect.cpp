
#include "Effect/AoEEffect.h"
#include "AoE/AoESpawningManager.h"
#include "AoE/AoEData.h"

UAoEEffect::UAoEEffect()
{

}

void UAoEEffect::ApplyEffect(AActor* TargetActor, const float& BaseDamage)
{
	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			if (UAoESpawningManager* AoESpawingManager = World->GetSubsystem<UAoESpawningManager>())
			{
				if (AoESpawingManager->SpawnAoEActor(0, FVector::ZeroVector))
				{
					UE_LOG(LogTemp, Warning, TEXT("Success Spawn AoE Actor"));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Not Spawn AoE Actor"));
				}
			}
		}
	}

	RemoveEffect();
}

void UAoEEffect::RemoveEffect()
{
	Super::RemoveEffect();
}

void UAoEEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UAoEEffect has been successfully collected by GC."));
}
