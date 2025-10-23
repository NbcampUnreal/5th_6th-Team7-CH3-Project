// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Effect/EffectApplyManager.h"
#include "ModeDefinition.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UModeDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FName ModeName;

	UPROPERTY(EditDefaultsOnly)
	struct FEffectStruct EffectStruct;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEffectBase> EffectClass;
};
