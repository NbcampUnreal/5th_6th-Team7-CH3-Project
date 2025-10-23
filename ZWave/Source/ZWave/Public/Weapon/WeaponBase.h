// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponDefinition.h"
#include "Engine/EngineTypes.h"
#include "WeaponBase.generated.h"

class ACharacter;

UCLASS(Abstract)
class ZWAVE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AWeaponBase();

public:
	virtual void Attack() PURE_VIRTUAL(AWeaponBase::Attack, ;);

	virtual bool Init(const UWeaponDefinition* WeaponDefinition) PURE_VIRTUAL(AWeaponBase::Init, return false;);

	virtual void Equip(ACharacter* NewOwner);

	virtual void Unequip();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool CanAttack() const { return bCanAttack; };

public:
	// BP 이펙트 처리용
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon", meta = (DisplayName = "OnFire"))
	void K2_OnFire(
		const TArray<FVector>& ImpactPositions,
		const TArray<FVector>& ImpactNormals,
		const TArray<TEnumAsByte<EPhysicalSurface>>& SurfaceTypes
	);

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> OwningCharacter;

	UPROPERTY(VisibleAnywhere)
	bool bEquipped = false;

	UPROPERTY(VisibleAnywhere)
	bool bCanAttack = false;
};
