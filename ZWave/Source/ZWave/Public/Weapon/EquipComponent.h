// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponDefinition.h"
#include "EquipComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
	First,
	Second,
	Third,
	None,
};

class AWeaponBase;

UCLASS(ClassGroup = (Equip), meta = (BlueprintSpawnableComponent))
class ZWAVE_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipComponent();

	bool Equip(EEquipSlot Slot);
	void UnEquip(EEquipSlot Slot);

	void SetSlotData(EEquipSlot Slot, const UWeaponDefinition* WeaponDef);
	void ClearSlotData(EEquipSlot Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AWeaponBase* GetCurrentWeapon() { return SlotMaps[CurrentSlot]; }

protected:
	virtual void BeginPlay() override;

	void AttachWeaponToOwner(AWeaponBase* Weapon, const UWeaponDefinition* WeaponDef);

protected:
	TMap<EEquipSlot, TObjectPtr<AWeaponBase>> SlotMaps;
	EEquipSlot CurrentSlot;

protected:
	// Equip Component에서 관리 (프로토 타입용)
	// 차후 이 클래스 대신 Inventory 등에서 대신 SetSlotData 등을 호출하여 바꿔준다
	// - 지금은 Slot 별 무기 데이터 고정
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EEquipSlot,TObjectPtr<UWeaponDefinition>> WeaponDefinitionMap;
};
