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

USTRUCT(BlueprintType)
struct FEffectStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	float EffectValue;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EEffectType> EffectType;
};


class UModeDefinition;
class UEffectBase;

UCLASS()
class ZWAVE_API UEffectApplyManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UEffectApplyManager();

	void ApplyEffect(AActor* Target);
	void RemoveAllEffect();
	void RemoveEffect(UEffectBase* RemoveClass);

private:
	UEffectBase* CreateEffect(const EEffectType& Type);
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EffectArray")
	TArray<UEffectBase*> CurrentApplyEffects;

};
