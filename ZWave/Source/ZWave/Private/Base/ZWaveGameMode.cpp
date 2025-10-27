// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/ZWaveGameMode.h"

#include "Kismet/GameplayStatics.h"

#include "Prop/PurificationDevice.h"

void AZWaveGameMode::BeginPlay()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APurificationDevice::StaticClass(), FoundActors);

	if (FoundActors.Num() == 1) {
		PuriDevice = static_cast<APurificationDevice*>(FoundActors[0]);
	}
}
