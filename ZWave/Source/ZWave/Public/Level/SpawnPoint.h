// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

class UArrowComponent;

UCLASS()
class ZWAVE_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnPoint();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UArrowComponent* ArrowVisualizer;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
