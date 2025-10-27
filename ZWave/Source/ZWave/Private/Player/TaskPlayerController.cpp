// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TaskPlayerController.h"
#include "EnhancedInputSubSystems.h"

ATaskPlayerController::ATaskPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	ShotAction(nullptr),
	SprintAction(nullptr),
	ShoulderAction(nullptr),
	ReloadAction(nullptr),
	EquipSlotFirstAction(nullptr),
	EquipSlotSecondAction(nullptr),
	EquipSlotThirdAction(nullptr)
{
}

void ATaskPlayerController::BeginPlay()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				EnhancedSubsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}