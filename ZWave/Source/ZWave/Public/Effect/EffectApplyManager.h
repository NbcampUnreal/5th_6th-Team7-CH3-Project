// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EffectApplyManager.generated.h"

class UModeDefinition;
class UEffectBase;
class IDamagable;
UCLASS()
class ZWAVE_API UEffectApplyManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UEffectApplyManager();

	void ApplyEffect(TScriptInterface<IDamagable> Target, const TArray<TSubclassOf<UEffectBase>>& EffectClassArray, const float& Duration = 0);
	void ApplyEffect(AActor* Causer, const TArray<TSubclassOf<UEffectBase>>& EffectClassArray, const float& Duration = 0);
private:
	UEffectBase* CreateEffect(TSubclassOf<UEffectBase> EffectClass);
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EffectArray")
	TArray<UEffectBase*> CurrentApplyEffects;
};
