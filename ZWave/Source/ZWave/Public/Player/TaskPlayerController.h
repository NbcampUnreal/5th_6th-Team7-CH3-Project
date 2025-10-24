// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
	UInputAction* ShotAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TaskPlayerController|Input")
	UInputAction* ShoulderAction;
};
