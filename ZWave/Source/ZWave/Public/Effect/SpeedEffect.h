// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/EffectBase.h"
#include "SpeedEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API USpeedEffect : public UEffectBase
{
	GENERATED_BODY()
	
public:
	USpeedEffect();

	virtual void ApplyEffect(AActor* TargetActor, const float& BaseDamage = 0) override;
	virtual void RemoveEffect() override;
	virtual void BeginDestroy() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect|Value")
	float SpeedMultiplier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect|Type")
	bool bIsDebuff;
}; 
