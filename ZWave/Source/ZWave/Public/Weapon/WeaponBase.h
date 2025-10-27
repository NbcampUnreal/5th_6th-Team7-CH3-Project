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

protected:
	FORCEINLINE bool IsDamagableActor(AActor* TargetActor) { return TargetActor && TargetActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()); }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> OwningCharacter;

	UPROPERTY(VisibleAnywhere)
	bool bEquipped = false;

	UPROPERTY(VisibleAnywhere)
	bool bCanAttack = true;
};
