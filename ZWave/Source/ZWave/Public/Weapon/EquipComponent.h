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

UENUM(BlueprintType)
enum class EConsumeSlot : uint8
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
	void AmmoSupply(float AddingPercent);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AWeaponBase* GetCurrentWeapon() { return SlotMaps[CurrentSlot]; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeaponBase* GetTargetWeapon(EEquipSlot Slot);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AWeaponBase* GetConsumeItem() { return ConsumeMaps[CurrentConsumeSlot]; }

	void EquipConsumeItem(EConsumeSlot Slot);

protected:
	virtual void BeginPlay() override;

	void AttachWeaponToOwner(AWeaponBase* Weapon, const UWeaponDefinition* WeaponDef);

	// 아이템은 슬롯 고정
	// 차후 ItemDef 부분이 있을시 수정 예정
	void SetSlotConsumeData(EConsumeSlot Slot, const UWeaponDefinition* WeaponDef);

protected:
	TMap<EEquipSlot, TObjectPtr<AWeaponBase>> SlotMaps;
	EEquipSlot CurrentSlot;

	// 차후 AWeaponBase 가 아니라 IConsumable 같은 용도로 바꿀 예정
	TMap<EConsumeSlot, TObjectPtr<AWeaponBase>> ConsumeMaps;
	EConsumeSlot CurrentConsumeSlot;

protected:
	// Equip Component에서 관리 (프로토 타입용)
	// 차후 이 클래스 대신 Inventory 등에서 대신 SetSlotData 등을 호출하여 바꿔준다
	// - 지금은 Slot 별 무기 데이터 고정
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EEquipSlot,TObjectPtr<UWeaponDefinition>> WeaponDefinitionMap;

	// 차후 ItemDefinition 등으로 바꿀 예정
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TMap<EConsumeSlot, TObjectPtr<UWeaponDefinition>> ConsumeDefinitionMap;
};
