// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/ZWaveGameMode.h"

#include "Prop/PurificationDevice.h"

void AZWaveGameMode::BeginPlay()
{
	PuriDevice = GetWorld()->SpawnActor<APurificationDevice>(PurificationDevice_class, FVector().ZeroVector, FRotator().ZeroRotator);
}
