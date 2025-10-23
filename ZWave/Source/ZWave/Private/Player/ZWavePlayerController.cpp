// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ZWavePlayerController.h"
#include "EnhancedInputSubSystems.h"

AZWavePlayerController::AZWavePlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	ShotAction(nullptr),
	SprintAction(nullptr),
	ShoulderAction(nullptr)
{
}

void AZWavePlayerController::BeginPlay()
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