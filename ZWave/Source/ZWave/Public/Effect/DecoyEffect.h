// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/EffectBase.h"
#include "DecoyEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UDecoyEffect : public UEffectBase
{
	GENERATED_BODY()

public:
	UDecoyEffect();

	virtual void ApplyEffect(AActor* TargetActor, const float& Duration) override;
	virtual void RemoveEffect() override;
	virtual void BeginDestroy() override;

protected:
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void SetDecoyTimer(const float& Duration);

private:
	FTimerHandle DecoyTimer;
};
