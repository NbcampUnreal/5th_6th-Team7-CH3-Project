// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TaskPlayerController.h"
#include "EnhancedInputSubSystems.h"
#include "Components/TextBlock.h"
#include "UI/ShopUI.h"

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
	ShopHUD(nullptr)
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
}

void ATaskPlayerController::ShowMainMenu()
{
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
	}

	if (IngameHUD)
	{
		IngameHUD->RemoveFromParent();
		IngameHUD = nullptr;
	}

	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
			bShowMouseCursor = true;
			SetInputMode(FInputModeUIOnly());
		}
	}
}

void ATaskPlayerController::ShowShopUI()
{
	if (ShopHUD)
	{
		UE_LOG(LogTemp, Warning, TEXT("RemovePreShopUI"));
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

void ATaskPlayerController::ShowGameHUD()
{

	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();
		MainMenuWidgetInstance = nullptr;
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
