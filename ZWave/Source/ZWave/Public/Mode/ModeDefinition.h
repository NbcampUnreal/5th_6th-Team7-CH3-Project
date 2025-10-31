// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Effect/EffectBase.h"
#include "Weapon/WeaponDefinition.h"
#include "ModeDefinition.generated.h"

UENUM(BlueprintType)
enum class EWeaponModifier : uint8
{
	EWM_Add,
	EWM_Percent,
	EWM_Multiple,
	EWM_None,
};

UCLASS()
class ZWAVE_API UModeDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	FName ModeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	EWeaponModifier ModeStatApplyType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	TObjectPtr<UWeaponDefinition> StatsDef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mode")
	TSubclassOf<UEffectBase> EffectClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UModingInstance> ModingClass;
};
