// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/ProjectileWeaponDefinition.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API AProjectileWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AProjectileWeapon();

public:
	virtual void Attack() override;

	virtual bool Init(const UWeaponDefinition* WeaponDefinition) override;

	void ThrowFromOwner();

protected:
	FVector ComputeThrowDirection() const;

	void AttachToOwner();

protected:
	UFUNCTION()
	void OnProjectileStop(const FHitResult& ImpactResult);

	// DamageCalculator 변환 전 테스트용 함수
	void DamageBoom();
	void Explode();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UNiagaraComponent> TrailComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Degree")
	float UpDegree;

	FProjectileWeaponStats ProjectileWeaponStat;
	FName AttachSocketName;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> OverlappedEnemies;

	FTimerHandle FuseTimer;
	FTimerHandle AttackTimer;
};
