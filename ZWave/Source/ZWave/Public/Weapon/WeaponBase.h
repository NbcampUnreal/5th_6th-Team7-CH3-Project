// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponDefinition.h"
#include "Engine/EngineTypes.h"
#include "Base/Damagable.h"
#include "Effect/EffectBase.h"
#include "WeaponBase.generated.h"

class ACharacter;
class UModingInstance;

UENUM(BlueprintType)
enum class EModingSlot : uint8
{
	EMS_First,
	EMS_Second,
	EMS_Third,
	EMS_Quad,
	EMS_Default,
};

UCLASS(Abstract)
class ZWAVE_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	virtual void Attack() PURE_VIRTUAL(AWeaponBase::Attack, ;);

	virtual bool Init(const UWeaponDefinition* WeaponDefinition) PURE_VIRTUAL(AWeaponBase::Init, return false;);

	virtual bool EquipModing(EModingSlot ModingSlot,UModingInstance* ModeInstance) PURE_VIRTUAL(AWeaponBase::EquipModing, return false;);

	virtual void UnEquipModing(EModingSlot ModingSlot) PURE_VIRTUAL(AWeaponBase::UnEquipModing,);

	virtual void Equip(ACharacter* NewOwner);

	virtual void Unequip();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool CanAttack() const { return bCanAttack; };

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UModingInstance* GetModing(EModingSlot ModingSlot);

protected:
	FORCEINLINE bool IsDamagableActor(AActor* TargetActor) { return TargetActor && TargetActor->GetClass()->ImplementsInterface(UDamagable::StaticClass()); }

	virtual void ApplyCurrentModing() PURE_VIRTUAL(AWeaponBase::ApplyCurrentModing, ;);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACharacter> OwningCharacter;

	UPROPERTY(VisibleAnywhere)
	bool bEquipped = false;

	UPROPERTY(VisibleAnywhere)
	bool bCanAttack = true;

	UPROPERTY(EditDefaultsOnly)
	TMap<EModingSlot, TObjectPtr<UModingInstance>> EquipModingMap;

	// 내부 Effect 관리용
	UPROPERTY(VisibleAnywhere)
	TMap<EModingSlot, TSubclassOf<UEffectBase>> EquipModingEffectClassMap;
};
