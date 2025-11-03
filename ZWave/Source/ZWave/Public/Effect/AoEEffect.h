// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/EffectBase.h"
#include "AoEEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UAoEEffect : public UEffectBase
{
	GENERATED_BODY()

public:
	UAoEEffect();

	virtual void ApplyEffect(AActor* AoECauser, const float& BaseDamage) override;
	virtual void RemoveEffect() override;
	virtual void BeginDestroy() override;
};
