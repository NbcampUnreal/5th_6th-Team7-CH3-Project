// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponBase.h"
#include "Weapon/MeleeWeaponDefinition.h"
#include "MeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API AMeleeWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	AMeleeWeapon();

public:
	virtual void Attack() override;

	virtual bool Init(const UWeaponDefinition* WeaponDefinition) override;

	virtual void Equip(ACharacter* NewOwner) override;

	virtual void Unequip() override;

	UFUNCTION(BlueprintCallable)
	void StartAttack();

	UFUNCTION(BlueprintCallable)
	void EndAttack();

protected:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<class USphereComponent> SphereCollision;

	FMeleeWeaponStats MeleeWeaponStat;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> OverlappedEnemies;

	FTimerHandle AttackTimer;
};
