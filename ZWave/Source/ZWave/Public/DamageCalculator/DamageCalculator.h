// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DamageCalculator.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UDamageCalculator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static float DamageCalculate(UObject* WorldContextObject, AActor* Target, float EffectValue, TSubclassOf<class UEffectBase> EffectClass);
	
};
