// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterActionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/ZWavePlayerCharacter.h"

UCharacterActionComponent::UCharacterActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterActionComponent::InitRefs(UCharacterMovementComponent* InMoveComp, USpringArmComponent* InSpringArm, UCameraComponent* InCamera)
{
	MoveComp = InMoveComp;
	SpringArm = InSpringArm;
	Camera = InCamera;

	if (SpringArm.IsValid())
	{
		NormalArmLength = SpringArm->TargetArmLength;
		NormalSocketOffsetY = SpringArm->SocketOffset.Y;
		SpringArm->SocketOffset = FVector{ 0.f,SpringArmNormalSocketOffsetY, SpringArmSocketOffsetZ };
	}
	if (MoveComp.IsValid())
	{
		NormalWalkSpeed = MoveComp->MaxWalkSpeed;
	}
}

void UCharacterActionComponent::Move(const FInputActionValue& Value)
{
	if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
	{
		const FVector2D Axis = Value.Get<FVector2D>();
		if (Axis.Y != 0.f)
		{
			MoveDirection = Axis.Y > 0.f ? ECharacterMoveDirection::Right : ECharacterMoveDirection::Left;
			OwnerChar->AddMovementInput(OwnerChar->GetActorRightVector(), Axis.Y);
		}
		if (Axis.X != 0.f)
		{
			MoveDirection = Axis.X > 0.f ? ECharacterMoveDirection::Foward : ECharacterMoveDirection::Back;
			OwnerChar->AddMovementInput(OwnerChar->GetActorForwardVector(), Axis.X);
		} 

		UE_LOG(LogTemp, Warning, TEXT("%d"),(MoveDirection));
		LastMoveInput = Axis;
	}
}

void UCharacterActionComponent::StopMove()
{
	LastMoveInput = FVector2D::ZeroVector;
	MoveDirection = ECharacterMoveDirection::Foward;
}

void UCharacterActionComponent::Look(const FInputActionValue& Value)
{
	if (APawn* OwnerPawn = Cast<APawn>(GetOwner()))
	{
		const FVector2D Axis = Value.Get<FVector2D>();
		OwnerPawn->AddControllerYawInput(Axis.X);
		OwnerPawn->AddControllerPitchInput(Axis.Y);
	}
}

void UCharacterActionComponent::StartJump()
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner())) Character->Jump();
}

void UCharacterActionComponent::StopJump()
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner())) Character->StopJumping();
}

void UCharacterActionComponent::StartSprint()
{
	if (bShoulder == false)
	{
		bIsSprinting = true;
	}
	else
	{
		bIsSprinting = false;
	}
}

void UCharacterActionComponent::StopSprint()
{
	bIsSprinting = false;
}

void UCharacterActionComponent::StartShoulder()
{
	SpringArm->SocketOffset = FVector{ 0.f,SpringArmShoulderSocketOffsetY, SpringArmSocketOffsetZ };
	MoveSpeed = ShoulderSpeed;
	MoveComp->MaxWalkSpeed = MoveSpeed;
	bShoulder = true;
}

void UCharacterActionComponent::StopShoulder()
{
	SpringArm->SocketOffset = FVector{ 0.f,SpringArmNormalSocketOffsetY, SpringArmSocketOffsetZ };
	MoveSpeed = NormalWalkSpeed;
	MoveComp->MaxWalkSpeed = MoveSpeed;
	bShoulder = false;
}

void UCharacterActionComponent::Shot()
{
}

void UCharacterActionComponent::TickAction(float DeltaTime)
{
	CheckMoveSpeed(DeltaTime);
	UpdateShoulder(DeltaTime);
	SetMeshDir(DeltaTime);
}

void UCharacterActionComponent::UpdateShoulder(float DeltaTime)
{
	if (bShoulder)
	{
		if (SpringArm->TargetArmLength > ShoulderArmLength)
		{
			SpringArm->TargetArmLength -= DeltaTime * ArmLerpSpeed;
		}
		else
		{
			SpringArm->TargetArmLength = ShoulderArmLength;
		}
	}
	else
	{
		if (SpringArm->TargetArmLength < NormalArmLength)
		{
			SpringArm->TargetArmLength += DeltaTime * ArmLerpSpeed;
		}
		else
		{
			SpringArm->TargetArmLength = NormalArmLength;
		}
	}
}

void UCharacterActionComponent::CheckMoveSpeed(float DeltaTime)
{
	if (bIsSprinting)
	{
		if (MoveSpeed < MaxSprintSpeed)
		{
			MoveSpeed += DeltaTime * SprintAnimationSpeed;
			MoveComp->MaxWalkSpeed = MoveSpeed;
		}
		else
		{
			MoveSpeed = MaxSprintSpeed;
			MoveComp->MaxWalkSpeed = MoveSpeed;
		}
	}
	else
	{
		if (MoveSpeed > NormalWalkSpeed)
		{
			MoveSpeed -= DeltaTime * SprintAnimationRecovorySpeed;
			MoveComp->MaxWalkSpeed = MoveSpeed;
		}
		else
		{
			MoveSpeed = NormalWalkSpeed;
			MoveComp->MaxWalkSpeed = MoveSpeed;
		}
	}
}

void UCharacterActionComponent::SetMeshDir(float DeltaTime)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !Character->GetMesh()) return;
	const bool bDiagX = FMath::Abs(LastMoveInput.X) > DiagonalDeadZone;
	const bool bDiagY = FMath::Abs(LastMoveInput.Y) > DiagonalDeadZone;
	const bool bIsDiagonal = bDiagX && bDiagY;

	float DesiredRelYaw = MeshYawOffsetDeg;

	if (bIsDiagonal)
	{
		const float RefYaw = Camera.IsValid() ? Camera->GetComponentRotation().Yaw : Character->GetActorRotation().Yaw;

		const FRotator CamYawRot(0.f, RefYaw, 0.f);
		const FVector Fwd = FRotationMatrix(CamYawRot).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(CamYawRot).GetUnitAxis(EAxis::Y);
		FVector WorldDir = Fwd * LastMoveInput.Y + Right * LastMoveInput.X;
		WorldDir.Z = 0.f;
		WorldDir.Normalize();
		const float MoveYaw = FRotationMatrix::MakeFromX(WorldDir).Rotator().Yaw;
		DesiredRelYaw = ConvertAngle(FMath::FindDeltaAngleDegrees(Character->GetActorRotation().Yaw, MoveYaw));
		LastDiagonalYawDeg = DesiredRelYaw;
	}
	else
	{
		DesiredRelYaw = 0.f;
	}

	MeshYawOffsetDeg = FMath::FInterpTo(MeshYawOffsetDeg, DesiredRelYaw + BaseMeshYawOffsetDeg, DeltaTime, MeshTurnSpeed);
	BP_MoveDirDeltaYawDeg = MeshYawOffsetDeg + 90.f;
	Character->GetMesh()->SetRelativeRotation(FRotator(0.f, MeshYawOffsetDeg, 0.f));

	UE_LOG(LogTemp, Warning, TEXT("BP_MoveDirDeltaYawDeg: %f"), BP_MoveDirDeltaYawDeg);
}

float UCharacterActionComponent::ConvertAngle(float InDeg)
{
	float Out = InDeg;

	if (InDeg > 90.f)
		Out = InDeg - 180.f;
	else if (InDeg < -90.f)
		Out = InDeg + 180.f;

	return Out;
}
