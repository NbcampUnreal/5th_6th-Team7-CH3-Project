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

	virtual void ApplyEffect(AActor* TargetActor, const float& BaseDamage = 0) override;
	virtual void RemoveEffect() override;
	virtual void BeginDestroy() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect|Value")
	float StaggerTime;

private:
	FTimerHandle Handle;

	float StaggerSpeedMultiplier;

	
};
