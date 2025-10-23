// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EffectBase.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UEffectBase : public UObject
{
	GENERATED_BODY()

public:
	UEffectBase();

	virtual void ApplyEffect(AActor* TargetActor, float Value);
	virtual void RemoveEffect();
	virtual void BeginDestroy() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target")
	AActor* Target;
};
