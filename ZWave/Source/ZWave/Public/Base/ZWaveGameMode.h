// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ZWaveGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API AZWaveGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APurificationDevice> PurificationDevice_class;
	class APurificationDevice* PuriDevice;

};
