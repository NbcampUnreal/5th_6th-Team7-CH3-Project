// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Base/Damagable.h"

#include "Turret.generated.h"

UCLASS()
class ZWAVE_API ATurret : public AActor, public IDamagable
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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UEquipComponent> EquipComp;


/// <summary>
/// 체력, 피격, 사망관련
/// </summary>
public:
	virtual void Attacked(AActor* DamageCauser, float Damage) override;
	virtual void ApplyDamage(float Damage, bool CheckArmor = true) override;
	virtual void Die() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere)
	float Health;

/// <summary>
/// 공격 관련
/// </summary>
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float AwarenessRange = 1000;

	class ABaseEnemy* Target;
	FTimerHandle AttackTimerHandle;

protected:
	virtual void Attack();
};
