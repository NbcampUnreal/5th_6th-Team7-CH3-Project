// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/EffectBase.h"
#include "StaggerEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UStaggerEffect : public UEffectBase
{
	GENERATED_BODY()

public:
	UStaggerEffect();

	virtual void ApplyEffect(AActor* TargetActor, float Value) override;
	virtual void RemoveEffect() override;

private:
	FTimerHandle Handle;

	float StaggerValue;
};
