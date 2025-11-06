// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseCharacter.h"
#include "BaseEnemy.generated.h"


UENUM(BlueprintType)
enum class EHitDir : uint8
{
	Front,
	Back,
	Left,
	Right
};

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

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UEnemyStateComponent> StateComp;

/// <summary>
/// 피격, 사망 관련
/// </summary>
protected:
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage|Hit")
	class UAnimMontage* FrontHitMontage;
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage|Hit")
	class UAnimMontage* BackHitMontage;
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage|Hit")
	class UAnimMontage* RightHitMontage;
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage|Hit")
	class UAnimMontage* LeftHitMontage;
	UPROPERTY(EditDefaultsOnly, Category = "AnimMontage|Hit")
	class UAnimMontage* DieMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundBase* HitSound;

private:
	UAnimMontage* GetAttackedMontage(EHitDir Direction);

public:
	virtual void Attacked(AActor* DamageCauser, float Damage) override;

	virtual void PlayHitAnimMontage(AActor* DamageCauser);
	virtual void CheckPriorityLv(AActor* DamageCauser);
	virtual void SetNewTarget(AActor* DamageCauser);

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

	/// <summary>
	/// 정화장치: 0, 터렛: 1, 플레이어: 2
	/// 
	/// if MaxAggroLv == 2 then 플레이어에게는 어그로 가 끌리지않음
	/// </summary>
	UPROPERTY(EditDefaultsOnly, Category = "Ability|AggroLv")
	int32 MaxPriorityLv;
	//int32 CurPriorityLv = 0;

public:
	int32 GetMaxPriorityLv() const;
	float GetAttackRange() const;
	float GetSightRange() const;

public:
	virtual void Attack();
};
