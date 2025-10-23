// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "CharacterActionComponent.generated.h"

class UCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum class ECharacterMoveDirection : uint8
{
	Foward,
	Back,
	Left,
	Right
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZWAVE_API UCharacterActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterActionComponent();

public:
	void InitRefs(UCharacterMovementComponent* InMoveComp, USpringArmComponent* InSpringArm, UCameraComponent* InCamera);

	void Move(const FInputActionValue& Value);
	void StopMove();
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void StartSprint();
	void StopSprint();
	void StartShoulder();
	void StopShoulder();
	void Shot();
	void TickAction(float DeltaTime);

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShoulder = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Facing")
	float BP_MoveDirDeltaYawDeg = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Facing")
	ECharacterMoveDirection MoveDirection = ECharacterMoveDirection::Foward;

private:
	TWeakObjectPtr<UCharacterMovementComponent> MoveComp;
	TWeakObjectPtr<USpringArmComponent>        SpringArm;
	TWeakObjectPtr<UCameraComponent>           Camera;

	float MaxSprintSpeed = 600.f;
	float NormalWalkSpeed = 300.f;
	float ShoulderSpeed = 150.f;

	float NormalArmLength = 300.f;
	float ShoulderArmLength = 150.f;
	float NormalSocketOffsetY = 0.f;
	float ArmLerpSpeed = 800.f;

	float SpringArmNormalSocketOffsetY = 42.f;
	float SpringArmShoulderSocketOffsetY = 55.f;
	float SpringArmSocketOffsetZ = 80.f;

	float SprintAnimationSpeed = 1000.f;
	float SprintAnimationRecovorySpeed = 1000.f;

	float MoveSpeed = 0.0f;

	float MeshTurnSpeed = 10.f;
	float FaceDirSpeedThreshold = 5.f;
	float BaseMeshYawOffsetDeg = -90.f;
	float LastDiagonalYawDeg = 0.f;
	float MeshYawOffsetDeg = 0.f;
	float DiagonalDeadZone = 0.1f;
	float MoveDirDeltaYawDeg = 0.f;
	FVector2D LastMoveInput = FVector2D::ZeroVector;

	void UpdateShoulder(float DeltaTime);
	void CheckMoveSpeed(float DeltaTime);
	void SetMeshDir(float DeltaTime);
	float ConvertAngle(float InDeg);
};
