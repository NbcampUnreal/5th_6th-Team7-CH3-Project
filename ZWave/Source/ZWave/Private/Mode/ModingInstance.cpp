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
	if (ModeDef->StatsDef != nullptr)
	{
		ModeStat = NewObject<UWeaponDefinition>(this, ModeDef->StatsDef);
	}
	else
	{
		ModeStat = nullptr;
	}
	
	ModeEffectClass = ModeDef->EffectClass;

	return true;
}