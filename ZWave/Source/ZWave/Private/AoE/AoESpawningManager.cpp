
#include "AoE/AoESpawningManager.h"
#include "AoE/AoEData.h"
#include "AoE/AoEActor.h"
#include "Engine/World.h"

void UAoESpawningManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAoEData* FireBombAoEData = LoadObject<UAoEData>(nullptr, TEXT("/Game/HyunJeongSeok/AoE/FireBombData.FireBombData"));
	UAoEData* PoisonAoEData = LoadObject<UAoEData>(nullptr, TEXT("/Game/HyunJeongSeok/AoE/PoisonData.PoisonData"));
	UBlueprint* BPAsset = LoadObject<UBlueprint>(nullptr, TEXT("/Game/HyunJeongSeok/BP_AoEActor.BP_AoEActor"));
	

	if (BPAsset)
	{
		SpawnActorClass = BPAsset->GeneratedClass;
	}

	if (FireBombAoEData)
	{
		AoEDataArray.Add(FireBombAoEData);
	}

	if (PoisonAoEData)
	{
		AoEDataArray.Add(PoisonAoEData);
	}
}

bool UAoESpawningManager::SpawnAoEActor(int32 index, FVector SpawnPoint)
{
	if (AoEDataArray.IsValidIndex(index))
	{
		AAoEActor* Actor = GetWorld()->SpawnActor<AAoEActor>(
			SpawnActorClass,
			SpawnPoint + FVector(0,0, 10.0f),
			FRotator::ZeroRotator
		);

		Actor->ActiveAoE(AoEDataArray[index]->ActiveTime, AoEDataArray[index]->DamagePerSec, AoEDataArray[index]->NiagaraParicle);

		return true;
	}

	return false;
}

void UAoESpawningManager::AddAoEData()
{
	
}
