// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "DamageCalculator.generated.h"


USTRUCT(BlueprintType)
struct FZWaveDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FZWaveDamageEvent() { DamageTypeClass = UDamageType::StaticClass(); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EffectArray")
	TArray<TSubclassOf<class UEffectBase>> EffectArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float BaseDamage;
};

class EffectBase;
class IDamagable;

UCLASS()
class ZWAVE_API UDamageCalculator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Calculate")
	static void DamageCalculate(
		UObject* WorldContextObject, 
		AActor* Causer, 
		TScriptInterface<IDamagable> Target,
		const float& BaseDamage,
		TArray<TSubclassOf<UEffectBase>>& EffectClassArray);

	UFUNCTION(BlueprintCallable, Category = "Calculate")
	static void DamageHelper(UObject* WorldContextObject, TScriptInterface<IDamagable> Target, AActor* DamageCauser, FZWaveDamageEvent const& DamageEvent);
};
