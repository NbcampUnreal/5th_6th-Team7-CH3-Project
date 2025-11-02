// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseAIController.h"
#include "RangedAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API ARangedAIController : public ABaseAIController
{
	GENERATED_BODY()

public:
	virtual FVector GetAttackLocation(FVector TargetLocation) override;

};
