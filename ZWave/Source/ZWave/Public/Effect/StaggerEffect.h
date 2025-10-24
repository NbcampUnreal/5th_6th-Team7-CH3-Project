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

	virtual float ApplyEffect(AActor* TargetActor, const float& BaseDamage, float Value) override;
	virtual void RemoveEffect() override;
	virtual void BeginDestroy() override;

private:
	FTimerHandle Handle;

	float StaggerValue;
};
