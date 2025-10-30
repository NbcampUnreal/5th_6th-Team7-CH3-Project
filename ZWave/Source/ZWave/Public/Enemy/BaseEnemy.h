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

	virtual void BeginPlay() override;

/// <summary>
/// 기본 영역
/// </summary>
public:
	void SetMoveSpeed(float MoveSpeed);
	float GetMoveSpeed();

/// <summary>
/// 피격, 사망 관련
/// </summary>
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage|Hit")
	class UAnimMontage* DieMontage;

public:
	virtual void Attacked(AActor* DamageCauser, float Damage) override;
	virtual void ApplyDamage(float Damage, bool CheckArmor = true) override;

	virtual void Die() override;

/// <summary>
/// 공격 관련
/// </summary>
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Attack")
	bool bIsAttackPlayerOnly;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Attack")
	bool bIsDumb;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Attack")
	float SightRange;
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Attack")
	float AttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage|Attack")
	class UAnimMontage* AttackMontage;
	
	/// <summary>
	/// Deprecated Variable
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Ability|Attack")
	bool bCanEditAttackPriority;

public:
	bool GetCanEditAttackPriority() const;
	float GetAttackRange() const;

public:
	virtual void Attack();
};
