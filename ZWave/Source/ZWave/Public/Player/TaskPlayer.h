// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseCharacter.h"
#include "InputActionValue.h"
#include "TaskPlayer.generated.h"

class UCharacterActionComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;

UCLASS()
class ZWAVE_API ATaskPlayer : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ATaskPlayer();

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attacked(AActor* DamageCauser, float Damage) override;
	virtual void Die() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskPlayer|Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskPlayer|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskPlayer|Action", meta = (AllowPrivateAccess = "true"))
	UCharacterActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskPlayer|AimSet", meta = (AllowPrivateAccess = "true"))
	float ControllerPitch = 0.0f;

};
