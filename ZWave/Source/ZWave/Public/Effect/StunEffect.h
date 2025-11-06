// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/EffectBase.h"
#include "StunEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UStunEffect : public UEffectBase
{
	GENERATED_BODY()

public:
	UStunEffect();

	virtual void ApplyEffect(AActor* TargetActor, const float& Duration) override;
	virtual void RemoveEffect() override;
	virtual void BeginDestroy() override;
};
