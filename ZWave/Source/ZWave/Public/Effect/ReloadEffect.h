// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Effect/EffectBase.h"
#include "ReloadEffect.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UReloadEffect : public UEffectBase
{
	GENERATED_BODY()

public:
	UReloadEffect();
	virtual void ApplyEffect(AActor* TargetActor, const float& BaseDamage) override;
	virtual void RemoveEffect() override;
	virtual void BeginDestroy() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effect|Value")
	float ReloadSpeedPer;
	
};
