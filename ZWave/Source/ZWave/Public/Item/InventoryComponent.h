// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemInstance.h"
#include "ItemDefinition.h"
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
	bool RemoveItem(const UItemDefinition* ItemDef, int32 Quantity);

	// Slot Idx 반환
	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 FindItem(const UItemDefinition* ItemDef) const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 CountItem(const UItemDefinition* ItemDef) const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryEntry> Entries;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxEntryCount;
};
