// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemInstance.h"
#include "ItemDefinition.h"
#include "Weapon/EquipComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FInventoryEntry
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsEmpty() const { return ItemInstance == nullptr; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory Slot")
	TObjectPtr<UItemInstance> ItemInstance = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const FInventoryEntry&, Entry);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZWAVE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory | Component")
	void AddItem(UItemDefinition* ItemDef, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Component")
	bool RemoveItemByDef(const UItemDefinition* ItemDef, int32 Quantity);

	UFUNCTION(BlueprintCallable, Category = "Inventory | Component")
	bool RemoveItemBySlot(int32 SlotIdx, int32 Quantity);

	// Slot Idx 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 FindItemByDef(const UItemDefinition* ItemDef) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const class UItemInstance* FindItemBySlot(int32 SlotIdx) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 CountItem(const UItemDefinition* ItemDef) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 CountItemByName(const FString& Name) const;

	// 직접적인 장착 X
	// EquipComponent에서 해당 슬롯에 장착 가능하도록 세팅
	UFUNCTION(BlueprintCallable)
	bool EquipWeaponItem(const UItemDefinition* ItemDef, EEquipSlot EquipSlot);

	UFUNCTION(BlueprintCallable)
	bool UnequipWeaponItem(const UItemDefinition* ItemDef, EEquipSlot EquipSlot);

	UFUNCTION(BlueprintCallable,BlueprintPure)
	TArray<int32> GetWeaponItemSlotIdxs() const;

	UFUNCTION(BlueprintCallable)
	bool EquipModingToWeapon(const UItemDefinition* WeaponItemDef, const UItemDefinition* ModingItemDef, int32 TargetEquipModingSlot);

	UFUNCTION(BlueprintCallable)
	bool UnequipModingToWeapon(const UItemDefinition* WeaponItemDef, int32 TargetUnEquipModingSlot);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetBioCoreCount() const { return BioCoreCount; };

	UFUNCTION(BlueprintCallable)
	void AddBioCoreCount(int32 Amount);

	UFUNCTION(BlueprintCallable)
	bool UseBioCoreCount(int32 Amount);

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

protected:
	// EquipSlot의 WeaponActor를 찾아 WeaponItem에 장착된 모드를 장착시켜줌
	void EquipModingOnWeaponActor(class UItemWeaponInstance* WeaponItem,EEquipSlot EquipSlot);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryEntry> Entries;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxEntryCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 BioCoreCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	FString DefaultWeaponName = "기본권총";
};
