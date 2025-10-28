// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterActionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/TaskPlayer.h"
#include "Weapon/ShootWeaponDefinition.h"

UCharacterActionComponent::UCharacterActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterActionComponent::InitRefs(UCharacterMovementComponent* InMoveComp, USpringArmComponent* InSpringArm, UCameraComponent* InCamera)
{
	MoveComp = InMoveComp;
	SpringArm = InSpringArm;
	Camera = InCamera;
	SpringArmNormalSocketOffsetY = SpringArmShoulderSocketOffsetY;

	if (SpringArm.IsValid())
	{
		//NormalArmLength = SpringArm->TargetArmLength;
		NormalSocketOffsetY = SpringArm->SocketOffset.Y;
		SpringArm->SocketOffset = FVector{ 0.f,SpringArmNormalSocketOffsetY, SpringArmSocketOffsetZ };
	}
	if (MoveComp.IsValid())
	{
		NormalWalkSpeed = MoveComp->MaxWalkSpeed * SpeedMultiply;
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
	if (bShoulder == false && bIsShooting == false)
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
	MoveComp->MaxWalkSpeed = MoveSpeed * SpeedMultiply;
	bShoulder = true;
}

void UCharacterActionComponent::StopShoulder()
{
	SpringArm->SocketOffset = FVector{ 0.f,SpringArmNormalSocketOffsetY, SpringArmSocketOffsetZ };
	MoveSpeed = NormalWalkSpeed;
	MoveComp->MaxWalkSpeed = MoveSpeed * SpeedMultiply;
	bShoulder = false;
}

void UCharacterActionComponent::Shooting(ATaskPlayer* OwnerChar, EShootType ShootType)
{
	MoveSpeed = NormalWalkSpeed;
	MoveComp->MaxWalkSpeed = MoveSpeed * SpeedMultiply;
	bIsShooting = true;

	if (OwnerChar)
	{
		if (CachedAnimInstance)
		{
			UnbindMontageNotifies(OwnerChar);
		}
		auto MeshCheck = OwnerChar->GetMesh();
		if (MeshCheck != nullptr)
		{
			if (UAnimInstance* Anim = MeshCheck->GetAnimInstance())
			{
				switch (ShootType)
				{
				case EShootType::ST_ShotHun:
				case EShootType::ST_Rifle:
				{

					const float PlayedLen = Anim->Montage_Play(
						RifleFireMontage,
						1.f,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);
					break;
				}
				case EShootType::ST_None:
				case EShootType::ST_HandHun:
				{
					const float PlayedLen = Anim->Montage_Play(
						PistolFireMontage,
						1.f,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);
					break;
				}
				default:
					break;
				}
			}
		}
	}
}

void UCharacterActionComponent::StopShooting()
{
	bIsShooting = false;
}

void UCharacterActionComponent::EquipChange(ATaskPlayer* OwnerChar, EShootType ShootType)
{
	if (OwnerChar)
	{
		auto MeshCheck = OwnerChar->GetMesh();
		if (MeshCheck != nullptr)
		{
			if (UAnimInstance* Anim = MeshCheck->GetAnimInstance())
			{
				ClearDryShotBlock();
				EnsureBindMontageNotifies(Anim);
				switch (ShootType)
				{
				case EShootType::ST_ShotHun:
				case EShootType::ST_Rifle:
				{
					const float PlayedLen = Anim->Montage_Play(
						RifleEquipMontage,
						1.f,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);
					break;
				}
				case EShootType::ST_None:
				case EShootType::ST_HandHun:
				{
					const float PlayedLen = Anim->Montage_Play(
						PistolEquipMontage,
						1.f,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);
					break;
				}
				default:
					break;
				}
			}
		}
	}
}

void UCharacterActionComponent::DryShot(ATaskPlayer* OwnerChar, EShootType ShootType)
{
	if (bDryShooting) return;

	if (OwnerChar)
	{
		if (CachedAnimInstance)
		{
			UnbindMontageNotifies(OwnerChar);
		}

		auto MeshCheck = OwnerChar->GetMesh();
		if (MeshCheck != nullptr)
		{
			bDryShooting = true;
			if (UAnimInstance* Anim = MeshCheck->GetAnimInstance())
			{
				float PlayedLen = 0.f;
				switch (ShootType)
				{
				case EShootType::ST_ShotHun:
				case EShootType::ST_Rifle:
				{
					PlayedLen = Anim->Montage_Play(
						RifleDryShotMontage,
						1.f,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);
					break;
				}
				case EShootType::ST_None:
				case EShootType::ST_HandHun:
				{
					PlayedLen = Anim->Montage_Play(
						PistolDryShotMontage,
						1.f,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);
					break;
				}
				default:
					break;
				}

				GetWorld()->GetTimerManager().SetTimer(
					DryShotBlockHandle,
					FTimerDelegate::CreateUObject(this, &UCharacterActionComponent::ClearDryShotBlock),
					PlayedLen,
					false
				);

			}
		}

	}
}

void UCharacterActionComponent::Reload(ATaskPlayer* OwnerChar, EShootType ShootType)
{
	if (OwnerChar)
	{
		if (CachedAnimInstance)
		{
			UnbindMontageNotifies(OwnerChar);
		}

		auto MeshCheck = OwnerChar->GetMesh();
		if (MeshCheck != nullptr)
		{
			if (UAnimInstance* Anim = MeshCheck->GetAnimInstance())
			{
				ClearDryShotBlock();
				switch (ShootType)
				{
				case EShootType::ST_ShotHun:
				case EShootType::ST_Rifle:
				{
					const float PlayedLen = Anim->Montage_Play(
						RifleReloadMontage,
						1.f,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);

					break;
				}
				case EShootType::ST_None:
				case EShootType::ST_HandHun:
				{
					const float PlayedLen = Anim->Montage_Play(
						PistolReloadMontage,
						1.f,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);
					break;
				}
				default:
					break;
				}
			}
		}
	}
}

void UCharacterActionComponent::TickAction(float DeltaTime)
{
	CheckMoveSpeed(DeltaTime);
	UpdateShoulder(DeltaTime);
	SetMeshDir(DeltaTime);
}

void UCharacterActionComponent::Attacked(AActor* DamageCauser)
{
	//맞는 방향에 맞춰서 애니메이션 재생
}

void UCharacterActionComponent::Die()
{
	//죽는 애니메이션 Montage
}

void UCharacterActionComponent::Grenade(ATaskPlayer* OwnerChar)
{
	if (OwnerChar)
	{
		if (CachedAnimInstance)
		{
			UnbindMontageNotifies(OwnerChar);
		}

		auto MeshCheck = OwnerChar->GetMesh();
		if (MeshCheck != nullptr)
		{
			if (UAnimInstance* Anim = MeshCheck->GetAnimInstance())
			{
				float const PlayedLen = Anim->Montage_Play(
					GrenadeMontage,
					1.f,
					EMontagePlayReturnType::MontageLength,
					0.f,
					true
				);
			}
		}
	}
}

void UCharacterActionComponent::ChangeSpeedMultiply(float Multiply)
{
	SpeedMultiply = Multiply;
	MoveComp->MaxWalkSpeed = MoveSpeed * SpeedMultiply;
}

void UCharacterActionComponent::EnsureBindMontageNotifies(UAnimInstance* Anim)
{
	if (!Anim) return;

	if (CachedAnimInstance && CachedAnimInstance != Anim)
	{
		UnbindMontageNotifies(Cast<ATaskPlayer>(GetOwner()));
	}

	if (!bNotifyBound)
	{
		CachedAnimInstance = Anim;
		bNotifyBound = true;
	}
}

void UCharacterActionComponent::UnbindMontageNotifies(ATaskPlayer* Player)
{
	CachedAnimInstance = nullptr;
	bNotifyBound = false;
	if(Player)
		Player->AttachWeaponTo(TEXT("WeaponSocket"));
}

void UCharacterActionComponent::OnMontageNotifyBegin(FName NotifyName)
{
	if (ATaskPlayer* Player = Cast<ATaskPlayer>(GetOwner()))
	{
		if (NotifyName == "WeaponL")
		{
			Player->AttachWeaponTo(TEXT("RifleMove"));
		}
		else if (NotifyName == "WeaponR")
		{
			UnbindMontageNotifies(Player);
		}
	}
}

void UCharacterActionComponent::ClearDryShotBlock()
{
	bDryShooting = false;
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(DryShotBlockHandle);
	}
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
			MoveComp->MaxWalkSpeed = MoveSpeed * SpeedMultiply;
		}
		else
		{
			MoveSpeed = MaxSprintSpeed;
			MoveComp->MaxWalkSpeed = MoveSpeed * SpeedMultiply;
		}
	}
	else
	{
		if (MoveSpeed > NormalWalkSpeed)
		{
			MoveSpeed -= DeltaTime * SprintAnimationRecovorySpeed;
			MoveComp->MaxWalkSpeed = MoveSpeed * SpeedMultiply;
		}
		else
		{
			MoveSpeed = NormalWalkSpeed;
			MoveComp->MaxWalkSpeed = MoveSpeed * SpeedMultiply;
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
