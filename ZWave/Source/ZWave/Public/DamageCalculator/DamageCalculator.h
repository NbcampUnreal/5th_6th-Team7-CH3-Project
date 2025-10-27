// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DamageCalculator.generated.h"

/**
 * 
 */
class EffectBase;

UCLASS()
class ZWAVE_API UDamageCalculator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Calculate")
	static void DamageCalculate(UObject* WorldContextObject, AActor* Causer, AActor* Target, const float& BaseDamage, float EffectValue, TSubclassOf<UEffectBase> EffectClass);
	
};
