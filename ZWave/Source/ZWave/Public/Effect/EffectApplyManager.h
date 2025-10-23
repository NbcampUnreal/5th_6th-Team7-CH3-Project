// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EffectApplyManager.generated.h"


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


class UModeDefinition;
class UEffectBase;

UCLASS()
class ZWAVE_API UEffectApplyManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UEffectApplyManager();

	void ApplyEffect(AActor* Target, float EffectValue, TSubclassOf<UEffectBase> EffectClass);

private:
	UEffectBase* CreateEffect(TSubclassOf<UEffectBase> EffectClass);
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EffectArray")
	TArray<UEffectBase*> CurrentApplyEffects;

};
