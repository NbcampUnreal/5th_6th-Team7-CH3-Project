// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ZWavePlayerCharacter.generated.h"

class UCharacterActionComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;

UCLASS()
class ZWAVE_API AZWavePlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AZWavePlayerCharacter();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test|Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test|Action", meta = (AllowPrivateAccess = "true"))
	UCharacterActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AimSet", meta = (AllowPrivateAccess = "true"))
	float ControllerPitch = 0.0f;

};
