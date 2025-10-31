// Fill out your copyright notice in the Description page of Project Settings.


#include "Mode/ModingInstance.h"

bool UModingInstance::Init(const UModeDefinition* ModeDef)
{
	if (ModeDef == nullptr)
	{
		return false;
	}

	ModeName = ModeDef->ModeName;
	ModeStatApplyType = ModeDef->ModeStatApplyType;
	ModeStat = NewObject<UWeaponDefinition>(this,ModeDef->StatsDef);
	ModeEffect = NewObject<UEffectBase>(this,ModeDef->EffectClass);

	return true;
}