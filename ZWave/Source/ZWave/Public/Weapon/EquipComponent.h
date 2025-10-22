// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponDefinition.h"
#include "EquipComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZWAVE_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipComponent();

protected:
	virtual void BeginPlay() override;

protected:
	// Equip Component에서 관리 (프로토 타입용)
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UWeaponDefinition>> WeaponDefinitionArray;


};
