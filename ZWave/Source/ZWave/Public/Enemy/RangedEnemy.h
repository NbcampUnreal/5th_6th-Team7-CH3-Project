// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "RangedEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API ARangedEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	ARangedEnemy();

	virtual void Attack() override;
};
