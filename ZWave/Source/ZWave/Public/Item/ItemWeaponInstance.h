// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemInstance.h"
#include "ItemWeaponInstance.generated.h"

class UItemModeInstance;

UCLASS()
class ZWAVE_API UItemWeaponInstance : public UItemInstance
{
	GENERATED_BODY()
	
public:

    UPROPERTY(BlueprintReadOnly, Category = "Weapon Instance")
    TArray<TObjectPtr<UItemModeInstance>> AttachedMods;

    UFUNCTION(BlueprintCallable, Category = "Weapon Instance")
    bool AttachMod(UItemModeInstance* ModeInstance);

    UFUNCTION(BlueprintCallable, Category = "Weapon Instance")
    void DetachMod(UItemModeInstance* ModInstance);

    UFUNCTION(BlueprintPure, Category = "Weapon Instance")
    bool HasEmptyModSlot() const;

    UFUNCTION(BlueprintPure, Category = "Weapon Instance")
    bool IsModAttached(UItemModeInstance* ModInstance) const;
};
