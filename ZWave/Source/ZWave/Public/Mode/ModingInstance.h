// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModeDefinition.h"
#include "ModingInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UModingInstance : public UObject
{
	GENERATED_BODY()
	
public:
	bool Init(const UModeDefinition* ModeDef);

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FName GetModename() const { return ModeName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EWeaponModifier GetModeApplyType() const { return ModeStatApplyType; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UWeaponDefinition* GetModeWeaponDef() const { return ModeStat; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UEffectBase* GetModeEffect() const { return ModeEffect; }

protected:
	FName ModeName;

	EWeaponModifier ModeStatApplyType;

	TObjectPtr<UWeaponDefinition> ModeStat;

	TObjectPtr<UEffectBase> ModeEffect;
};
