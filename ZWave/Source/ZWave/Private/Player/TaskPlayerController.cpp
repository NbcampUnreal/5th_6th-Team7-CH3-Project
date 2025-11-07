// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TaskPlayerController.h"
#include "EnhancedInputSubSystems.h"
#include "Components/TextBlock.h"
#include "UI/ShopUI.h"
#include "UI/IngamePauseMenu.h"
#include "Base/ZWaveGameState.h"
#include "Level/ZWaveTypes.h"

ATaskPlayerController::ATaskPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	ShootingAction(nullptr),
	SprintAction(nullptr),
	ShoulderAction(nullptr),
	ReloadAction(nullptr),
	EquipSlotFirstAction(nullptr),
	EquipSlotSecondAction(nullptr),
	EquipSlotThirdAction(nullptr),
	GrenadeAction(nullptr),
	IngameHUDClass(nullptr),
	ShopHUDClass(nullptr),
	IngameHUD(nullptr),
	ShopHUD(nullptr), 
	ActiveAction(nullptr)
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

	FString CurrentMapName = GetWorld()->GetMapName();
	if (CurrentMapName.Contains("TitleLevel"))
	{
		ShowMainMenu();
	}
	else
	{
		ShowGameHUD();
	}

	GameState = GetWorld()->GetGameState<AZWaveGameState>();
}

void ATaskPlayerController::ShowMainMenu()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}

	if (IngameHUD)
	{
		IngameHUD->RemoveFromParent();
		IngameHUD = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATaskPlayerController::ShowShopUI()
{
	if (EGameState::EGS_Preparing != GameState->GetCurrentGameState())
	{
		return;
	}

	if (ShopHUD)
	{
		ShopHUD->RemoveFromParent();
		ShopHUD = nullptr;
	}

	if (ShopHUDClass)
	{
		ShopHUD = CreateWidget<UShopUI>(this, ShopHUDClass);
		if (ShopHUD)
		{
			ShopHUD->ActivateWidget();
			SetPause(true);
			ShopHUD->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}

}

void ATaskPlayerController::ShowResultHUD()
{
	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}

	if (IngameHUD)
	{
		IngameHUD->RemoveFromParent();
		IngameHUD = nullptr;
	}

	if (ShopHUD)
	{
		ShopHUD->RemoveFromParent();
		ShopHUD = nullptr;
	}

	if (ResultWidgetClass)
	{
		ResultWidget = CreateWidget<UIngamePauseMenu>(this, ResultWidgetClass);
		if (ResultWidget)
		{
			SetPause(true);
			ResultWidget->AddToViewport();
			ResultWidget->OnGameOver(false);
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATaskPlayerController::InitHP(float MaxHealth)
{
	if (IngameHUD)
	{
		IngameHUD->InitPlayerHealthBar(MaxHealth);
	}
}

void ATaskPlayerController::ShowGameHUD()
{

	if (MainMenuWidget)
	{
		MainMenuWidget->RemoveFromParent();
		MainMenuWidget = nullptr;
	}

	if (IngameHUD)
	{
		IngameHUD->RemoveFromParent();
		IngameHUD = nullptr;
	}

	if (IngameHUDClass)
	{
		IngameHUD = CreateWidget<UIngameHUD>(this, IngameHUDClass);
		if (IngameHUD)
		{
			IngameHUD->AddToViewport();
			bShowMouseCursor = false;
			SetInputMode(FInputModeGameOnly());

			IngameHUD->OnEnemyCountChanged(0, 0);
		}
	}
}
