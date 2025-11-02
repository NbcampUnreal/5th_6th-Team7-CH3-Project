// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy.h"
#include "BombZombie.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API ABombZombie : public ABaseEnemy
{
	GENERATED_BODY()

public:
	virtual void Die() override;

	virtual void Attack() override;
	void Bomb();
	
};
