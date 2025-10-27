// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Effect/EffectBase.h"
#include "ModeDefinition.generated.h"


UENUM(BlueprintType)
enum EEffectType : uint8
{
	None,
	Stagger
};

UENUM(BlueprintType)
enum EAbilityType : uint8
{
	Active,
	Passive
};

USTRUCT(BlueprintType)
struct FEffectStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	float EffectValue;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EEffectType> EffectType;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EAbilityType> AbilityType;
};


UCLASS()
class ZWAVE_API UModeDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FName ModeName;

	UPROPERTY(EditDefaultsOnly)
	struct FEffectStruct EffectStruct;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UEffectBase> EffectClass;
};
