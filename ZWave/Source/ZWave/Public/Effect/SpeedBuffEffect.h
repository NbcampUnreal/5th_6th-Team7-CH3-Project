// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/EffectBase.h"
#include "SpeedBuffEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API USpeedBuffEffect : public UEffectBase
{
	GENERATED_BODY()

public:
	USpeedBuffEffect();

	virtual void ApplyEffect(AActor* TargetActor, const float& Duration) override;
	virtual void RemoveEffect() override;
	virtual void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = "BuffTimer")
	void SetBuffTimer(const float& Duration);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect|Value")
	float SpeedMultiplier;
	
private:
	FTimerHandle BuffTimer;
};
