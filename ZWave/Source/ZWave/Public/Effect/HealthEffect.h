// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/EffectBase.h"
#include "HealthEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UHealthEffect : public UEffectBase
{
	GENERATED_BODY()

public:
	UHealthEffect();
	virtual void ApplyEffect(AActor* TargetActor, const float& Duration) override;
	virtual void RemoveEffect() override;
	virtual void BeginDestroy() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect|Value")
	float HealAmount;
};
