// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"

#include "Base/Damagable.h"

#include "Turret.generated.h"

UCLASS()
class ZWAVE_API ATurret : public AActor, public IDamagable, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	ATurret();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSphereBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:
	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<class UCapsuleComponent> CapsuleComp;

	UPROPERTY(Category = "Components", VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = 0;


/// <summary>
/// 체력, 피격, 사망관련
/// </summary>
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> Montage_Destory;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere)
	float Health;

public:
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Attacked(AActor* DamageCauser, float Damage) override;
	virtual void ApplyDamage(float Damage, bool CheckArmor = true) override;
	virtual void Die() override;

/// <summary>
/// 공격 관련, EquipComp에서 SetSlotData에서 GetPawn()을 실행해서, AWeaponBase와 WeaponDefinition 을 사용할수 없음.. 
/// </summary>
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<class UAnimMontage> AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float AwarenessRange = 1000;
	UPROPERTY(EditAnywhere, Category = "Ability")
	float RotationSpeed = 500;
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float WeaponDamage = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float WeaponRPM = 600;
	float FireInterval = 0;

	//class AWeaponBase* Weapon;

	class ABaseEnemy* Target;
	FTimerHandle AttackTimerHandle;

	bool bShouldRot = false;

protected:
	virtual void Attack();
	virtual void RotateToTarget(float DeltaTime);
	virtual void StopAttack();
	virtual void SearchEnemy();
};
