// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemInstance.h"
#include "ItemModeInstance.generated.h"

class UItemWeaponInstance;


UCLASS(BlueprintType)
class ZWAVE_API UItemModeInstance : public UItemInstance
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Mod Instance")
    TObjectPtr<UItemWeaponInstance> EquippedWeapon = nullptr;

    UFUNCTION(BlueprintPure, Category = "Mod Instance")
    bool IsEquipped() const;

    void OnEquipped(UItemWeaponInstance* TargetWeapon);
    void OnUnequipped();
};
