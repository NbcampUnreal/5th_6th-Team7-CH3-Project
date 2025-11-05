
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

bool UAoESpawningManager::SpawnAoEActor(AActor* Causer, int32 index, FVector SpawnPoint)
{
	if (AoEDataArray.IsValidIndex(index))
	{
		FActorSpawnParameters SpawnParam;
		SpawnParam.Owner = Causer;
		SpawnParam.Instigator = Cast<APawn>(Causer);


		AAoEActor* AoE = GetWorld()->SpawnActor<AAoEActor>(
			SpawnActorClass,
			SpawnPoint, // 임시 포지션
			FRotator::ZeroRotator,
			SpawnParam
		);


		AoE->ActiveAoE(
			AoEDataArray[index]->NiagaraParicle,
			AoEDataArray[index]->DamageParam
		);
		return true;
	}

	return false;
}

void UAoESpawningManager::AddAoEData()
{
	
}
