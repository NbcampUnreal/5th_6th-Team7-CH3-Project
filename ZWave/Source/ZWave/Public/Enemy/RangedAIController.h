// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/PlayerTargetBaseAIController.h"
#include "RangedAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API ARangedAIController : public APlayerTargetBaseAIController
{
	GENERATED_BODY()

public:
	virtual bool CheckCondition(AActor* Target) override;
};
