// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ZWavePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ZWAVE_API AZWavePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AZWavePlayerController();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestPlayerController|Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestPlayerController|Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestPlayerController|Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestPlayerController|Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestPlayerController|Input")
	UInputAction* ShotAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestPlayerController|Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TestPlayerController|Input")
	UInputAction* ShoulderAction;
};
