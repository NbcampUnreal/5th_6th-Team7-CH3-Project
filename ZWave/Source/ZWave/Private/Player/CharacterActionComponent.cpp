// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CharacterActionComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Player/TaskPlayer.h"
#include "Weapon/ShootWeaponDefinition.h"
#include <Kismet/KismetSystemLibrary.h>

UCharacterActionComponent::UCharacterActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCharacterActionComponent::InitRefs(ATaskPlayer* InPlayerCharacter, UCharacterMovementComponent* InMoveComp, USpringArmComponent* InSpringArm, UCameraComponent* InCamera)
{
	MoveComp = InMoveComp;
	SpringArm = InSpringArm;
	Camera = InCamera;
	OwnerCharacter = InPlayerCharacter;
	SpringArmNormalSocketOffsetY = SpringArmShoulderSocketOffsetY;

	if (SpringArm.IsValid())
	{
		NormalSocketOffsetY = SpringArm->SocketOffset.Y;
		SpringArm->SocketOffset = FVector{ 0.f,SpringArmNormalSocketOffsetY, SpringArmSocketOffsetZ };
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
	MoveComp->MaxWalkSpeed = GetSpeedValue(MoveSpeed); 
	bShoulder = true;
}

void UCharacterActionComponent::StopShoulder()
{
	SpringArm->SocketOffset = FVector{ 0.f,SpringArmNormalSocketOffsetY, SpringArmSocketOffsetZ };
	MoveSpeed = NormalWalkSpeed;
	MoveComp->MaxWalkSpeed = GetSpeedValue(MoveSpeed); 
	bShoulder = false;
}

void UCharacterActionComponent::Shooting(EShootType ShootType, float ShootSpeedMultiply)
{
	MoveSpeed = NormalWalkSpeed;
	MoveComp->MaxWalkSpeed = GetSpeedValue(MoveSpeed);
	bIsShooting = true;

	if (OwnerCharacter.Get())
	{
		if (CachedAnimInstance)
		{
			UnbindMontageNotifies();
		}
		auto MeshCheck = OwnerCharacter->GetMesh();
		if (MeshCheck != nullptr)
		{
			if (UAnimInstance* Anim = MeshCheck->GetAnimInstance())
			{
				switch (ShootType)
				{
				case EShootType::ST_ShotHun:
				{
					Anim->Montage_Play(
						ShotgunFireMontage,
						ShootSpeedMultiply,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);
					break;
				}
				case EShootType::ST_Rifle:
				{
					Anim->Montage_Play(
						RifleFireMontage,
						ShootSpeedMultiply,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);
					break;
				}
				case EShootType::ST_None:
				case EShootType::ST_HandHun:
				{
					Anim->Montage_Play(
						PistolFireMontage,
						ShootSpeedMultiply,
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

void UCharacterActionComponent::EquipChange(EShootType ShootType)
{
	if (OwnerCharacter.Get())
	{
		auto MeshCheck = OwnerCharacter->GetMesh();
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
					Anim->Montage_Play(
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
					Anim->Montage_Play(
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

void UCharacterActionComponent::DryShot(EShootType ShootType)
{
	if (bDryShooting) return;

	if (OwnerCharacter.Get())
	{
		if (CachedAnimInstance)
		{
			UnbindMontageNotifies();
		}

		auto MeshCheck = OwnerCharacter->GetMesh();
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

void UCharacterActionComponent::Reload(EShootType ShootType, float ReloadSpeedMultiply)
{
	if (OwnerCharacter.Get())
	{
		if (CachedAnimInstance)
		{
			UnbindMontageNotifies();
		}

		auto MeshCheck = OwnerCharacter->GetMesh();
		if (MeshCheck != nullptr)
		{
			if (UAnimInstance* Anim = MeshCheck->GetAnimInstance())
			{
				ClearDryShotBlock();
				switch (ShootType)
				{
				case EShootType::ST_ShotHun:
				{
					const float PlayedLen = Anim->Montage_Play(
						ShotgunReloadMontage,
						ReloadSpeedMultiply,
						EMontagePlayReturnType::MontageLength,
						0.f,
						true
					);

					break;
				}
				case EShootType::ST_Rifle:
				{
					const float PlayedLen = Anim->Montage_Play(
						RifleReloadMontage,
						ReloadSpeedMultiply,
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
						ReloadSpeedMultiply,
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
	if (!IsValid(DamageCauser))
		return;

	if (OwnerCharacter.Get())
	{
		FVector SelfLocation = OwnerCharacter->GetActorLocation();
		FVector CauserLocation = DamageCauser->GetActorLocation();
		FVector ToCauser = (CauserLocation - SelfLocation).GetSafeNormal();
		FVector Forward = OwnerCharacter->GetActorForwardVector();
		FVector Right = OwnerCharacter->GetActorRightVector();

		float ForwardDot = FVector::DotProduct(Forward, ToCauser);
		float RightDot = FVector::DotProduct(Right, ToCauser);

		FString HitDirection;
		UAnimMontage* PlayAnim = nullptr;

		if (ForwardDot > 0.707f)
		{
			PlayAnim = GetAttackedMontage(EHitDirection::Front);
		}
		else if (ForwardDot < -0.707f)
		{
			PlayAnim = GetAttackedMontage(EHitDirection::Back);
		}
		else if (RightDot > 0)
		{
			PlayAnim = GetAttackedMontage(EHitDirection::Right);
		}
		else
		{
			PlayAnim = GetAttackedMontage(EHitDirection::Left);
		}

		auto MeshCheck = OwnerCharacter->GetMesh();
		if (PlayAnim != nullptr && MeshCheck != nullptr)
		{
			if (UAnimInstance* Anim = MeshCheck->GetAnimInstance())
			{
				float const PlayedLen = Anim->Montage_Play(
					PlayAnim,
					1.f,
					EMontagePlayReturnType::MontageLength,
					0.f,
					true
				);
			}
		}
	}
}


UAnimMontage* UCharacterActionComponent::GetAttackedMontage(EHitDirection Direction)
{
	switch (Direction)
	{
	case EHitDirection::Front:
		return FrontHitMontage.Get();
	case EHitDirection::Back:
		return BackHitMontage.Get();
	case EHitDirection::Left:
		return LeftHitMontage.Get();
	case EHitDirection::Right:
		return RightHitMontage.Get();
	default:
		return nullptr;
	}
}

float UCharacterActionComponent::GetSpeedValue(float Speed)
{
	return Speed * SpeedMultiply * OwnerCharacter->GetSpeedMultiply();
}

void UCharacterActionComponent::Die()
{
	if (bIsDead) return;

	if (OwnerCharacter.Get())
	{
		auto MeshCheck = OwnerCharacter->GetMesh();
		if (MeshCheck != nullptr)
		{
			if (UAnimInstance* Anim = MeshCheck->GetAnimInstance())
			{
				bIsDead = true;

				OwnerCharacter->GetCharacterMovement()->DisableMovement();
				OwnerCharacter->DisableInput(Cast<APlayerController>(OwnerCharacter->GetController()));
				OwnerCharacter->bUseControllerRotationYaw = false;

				float const PlayedLen = Anim->Montage_Play(
					DeathMontage,
					1.f,
					EMontagePlayReturnType::MontageLength,
					0.f,
					true
				);

				GetWorld()->GetTimerManager().SetTimer(
					StopMotionHandler,
					FTimerDelegate::CreateUObject(this, &UCharacterActionComponent::SetStopMotion),
					PlayedLen -0.1f,
					false
				);

				GetWorld()->GetTimerManager().SetTimer(
					DeathHandler,
					FTimerDelegate::CreateUObject(this, &UCharacterActionComponent::GameOver),
					PlayedLen + 2.f,
					false
				);
			}
		}
	}
}

void UCharacterActionComponent::Grenade()
{
	if (OwnerCharacter.Get())
	{
		if (CachedAnimInstance)
		{
			UnbindMontageNotifies();
		}

		auto MeshCheck = OwnerCharacter->GetMesh();
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

void UCharacterActionComponent::ActiveFuildObject()
{
	if (OwnerCharacter.Get())
	{
		if (CachedAnimInstance)
		{
			UnbindMontageNotifies();
		}

		auto MeshCheck = OwnerCharacter->GetMesh();
		if (MeshCheck != nullptr)
		{
			if (UAnimInstance* Anim = MeshCheck->GetAnimInstance())
			{
				float const PlayedLen = Anim->Montage_Play(
					ActiveMontage,
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
	MoveComp->MaxWalkSpeed = GetSpeedValue(MoveSpeed);
}

void UCharacterActionComponent::EnsureBindMontageNotifies(UAnimInstance* Anim)
{
	if (!Anim) return;

	if (CachedAnimInstance && CachedAnimInstance != Anim)
	{
		UnbindMontageNotifies();
	}

	if (!bNotifyBound)
	{
		CachedAnimInstance = Anim;
		bNotifyBound = true;
	}
}

void UCharacterActionComponent::UnbindMontageNotifies()
{
	CachedAnimInstance = nullptr;
	bNotifyBound = false;
	if(OwnerCharacter.Get())
		OwnerCharacter->AttachWeaponTo(TEXT("WeaponSocket"));
}

void UCharacterActionComponent::OnMontageNotifyBegin(FName NotifyName)
{
	if (OwnerCharacter.Get())
	{
		if (NotifyName == "WeaponL")
		{
			OwnerCharacter->AttachWeaponTo(TEXT("RifleMove"));
		}
		else if (NotifyName == "WeaponR")
		{
			UnbindMontageNotifies();
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

void UCharacterActionComponent::SetStopMotion()
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character)
	{
		if (USkeletalMeshComponent* MeshComp = Character->GetMesh())
		{
			MeshComp->bPauseAnims = true; 
			MeshComp->SetComponentTickEnabled(false);
		}
	}
}

void UCharacterActionComponent::GameOver()
{
	ATaskPlayer* Character = Cast<ATaskPlayer>(GetOwner());
	if (Character)
	{
		Character->GameOver();

	}
	//UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);

	

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
			MoveComp->MaxWalkSpeed = GetSpeedValue(MoveSpeed);
		}
		else
		{
			MoveSpeed = MaxSprintSpeed;
			MoveComp->MaxWalkSpeed = GetSpeedValue(MoveSpeed);
		}
	}
	else
	{
		if (MoveSpeed > NormalWalkSpeed)
		{
			MoveSpeed -= DeltaTime * SprintAnimationRecovorySpeed;
			MoveComp->MaxWalkSpeed = GetSpeedValue(MoveSpeed); 
		}
		else
		{
			MoveSpeed = NormalWalkSpeed;
			MoveComp->MaxWalkSpeed = GetSpeedValue(MoveSpeed); 
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
