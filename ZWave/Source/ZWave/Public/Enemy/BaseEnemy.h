// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseCharacter.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

public:
	virtual void Attacked(AActor* DamageCauser, float Damage) override;
	virtual void ApplyDamage(float Damage, bool CheckArmor = true) override;

	virtual void Die() override;

public:
	void SetMoveSpeed(float MoveSpeed);
	float GetMoveSpeed();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float AttackRange;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bCanEditAttackPriority;

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage")
	class UAnimMontage* AttackMontage;

public:
	bool GetCanEditAttackPriority() const;
	float GetAttackRange() const;

public:
	virtual void Attack();
};
