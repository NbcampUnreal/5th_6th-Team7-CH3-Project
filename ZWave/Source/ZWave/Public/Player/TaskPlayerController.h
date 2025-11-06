// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UI/IngameHUD.h"
#include "TaskPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ZWAVE_API ATaskPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ATaskPlayerController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UIngameHUD> IngameHUDClass;  

	UPROPERTY()
	UIngameHUD* IngameHUD;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UShopUI> ShopHUDClass;

	UPROPERTY()
	UShopUI* ShopHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* ShootingAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* ShoulderAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* ReloadAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* GrenadeAction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* EquipSlotFirstAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* EquipSlotSecondAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* EquipSlotThirdAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* ShopUIAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* ActiveAction;


	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();

	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu();

	UFUNCTION(BlueprintCallable, Category = "Shop")
	void ShowShopUI();
};
