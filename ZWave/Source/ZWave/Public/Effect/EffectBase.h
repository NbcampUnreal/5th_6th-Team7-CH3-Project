// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EffectBase.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ZWAVE_API UEffectBase : public UObject
{
	GENERATED_BODY()

public:
	virtual float ApplyEffect(AActor* TargetActor, const float& BaseDamage, float Value) PURE_VIRTUAL(UEffectBase::ApplyEffect, return BaseDamage;);
	virtual void RemoveEffect() PURE_VIRTUAL(UEffectBase::RemoveEffect, );

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target")
	AActor* Target;
};
