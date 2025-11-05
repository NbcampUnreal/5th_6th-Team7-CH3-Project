// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/EffectBase.h"
#include "GainItemEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UGainItemEffect : public UEffectBase
{
	GENERATED_BODY()

public:
	UGainItemEffect();
	virtual void ApplyEffect(AActor* TargetActor, const float& Duration) override;
	virtual void ApplyEffect(AActor* TargetActor, const int32& Itemindex) override;
	virtual void RemoveEffect() override;
	virtual void BeginDestroy();

	UFUNCTION(BlueprintCallable, Category = "Init")
	void Init();
};
