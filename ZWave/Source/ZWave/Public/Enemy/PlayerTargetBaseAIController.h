// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseAIController.h"
#include "PlayerTargetBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API APlayerTargetBaseAIController : public ABaseAIController
{
	GENERATED_BODY()

public:
	virtual bool CheckCondition(AActor* Target);
};
