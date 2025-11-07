// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TaskPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"   
#include "GameFramework/SpringArmComponent.h"
#include "Player/CharacterActionComponent.h" 
#include "EnhancedInputComponent.h"
#include "Player/TaskPlayerController.h"
#include "Weapon/EquipComponent.h"
#include "Item/InventoryComponent.h"
#include "Weapon/ShootWeapon.h"
#include "UI/IngameHUD.h"
#include "Base/ZWaveGameState.h"
#include "Player/InteractInterface.h"

ATaskPlayer::ATaskPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	Health = 100;
	MaxHealth = 100;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	ActionComp = CreateDefaultSubobject<UCharacterActionComponent>(TEXT("ActionComp"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	GetCharacterMovement()->MaxWalkSpeed = 300.f;

	TeamID = 1;

	MoveSpeedMultiply = 1.f;
	ReloadSpeedMultiply = 1.f;
	ShotSpeedMultiplay = 1.f;
}

void ATaskPlayer::AttachWeaponTo(const FName SocketName)
{
	if (NowShootWeapon)
	{
		USkeletalMeshComponent* SkeletalMesh = GetMesh();
		if (SkeletalMesh == nullptr)
			return;

		if (NowShootWeapon->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon Attached Failed! SocketName : %s"), *SocketName.ToString());
		}
	}
}

void ATaskPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	if (ActionComp && EquipComponent)
	{
		ActionComp->InitRefs(this, GetCharacterMovement(), SpringArmComp, CameraComp);

		if (EquipComponent->Equip(EEquipSlot::First))
		{
			EquipChange();
;		}
		else
		{
			if (AWeaponBase* BaseWeapon = EquipComponent->GetCurrentWeapon())
			{
				AShootWeapon* NewShootWeapon = Cast<AShootWeapon>(BaseWeapon);

				if (NowShootWeapon)
				{
					NowShootWeapon->OnFireSuccess.RemoveDynamic(this, &ATaskPlayer::ShootingAction);
					NowShootWeapon = nullptr;
				}

				if (NewShootWeapon)
				{
					NowShootWeapon = NewShootWeapon;
					NowShootWeapon->OnFireSuccess.AddUniqueDynamic(this, &ATaskPlayer::ShootingAction);
					ActionComp->UnbindMontageNotifies();
				}
			}
		}
	}
}

void ATaskPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (ActionComp)
	{
		ActionComp->TickAction(DeltaTime);
	}

	float Pitch = GetController()->GetControlRotation().Pitch;
	float Yaw = GetController()->GetControlRotation().Yaw;
	if (Pitch <= 180.f)
		ControllerPitch = Pitch;
	else
		ControllerPitch = Pitch - 360.f;

}

void ATaskPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ATaskPlayerController* PlayerController = Cast<ATaskPlayerController>(GetController()))
		{
			if (PlayerController->ReloadAction)
			{
				EnhancedInput->BindAction(PlayerController->ReloadAction,
					ETriggerEvent::Triggered,
					this,
					&ATaskPlayer::Reload
				);
			}
			if (PlayerController->EquipSlotFirstAction)
			{
				EnhancedInput->BindAction(PlayerController->EquipSlotFirstAction,
					ETriggerEvent::Triggered,
					this,
					&ATaskPlayer::EquipFirstSlot
				);
			}
			if (PlayerController->EquipSlotSecondAction)
			{
				EnhancedInput->BindAction(PlayerController->EquipSlotSecondAction,
					ETriggerEvent::Triggered,
					this,
					&ATaskPlayer::EquipSecondSlot
				);
			}
			if (PlayerController->EquipSlotThirdAction)
			{
				EnhancedInput->BindAction(PlayerController->EquipSlotThirdAction,
					ETriggerEvent::Triggered,
					this,
					&ATaskPlayer::EquipThirdSlot
				);
			}
			if (PlayerController->ShopUIAction)
			{
				EnhancedInput->BindAction(PlayerController->ShopUIAction,
					ETriggerEvent::Triggered,
					PlayerController,
					&ATaskPlayerController::ShowShopUI
				);
			}
			if (PlayerController->ActiveAction)
			{
				EnhancedInput->BindAction(PlayerController->ActiveAction,
					ETriggerEvent::Triggered,
					this,
					&ATaskPlayer::ActiveFieldObject
				);
			}

			if (ActionComp)
			{
				if (PlayerController->MoveAction)
				{
					EnhancedInput->BindAction(PlayerController->MoveAction,
						ETriggerEvent::Triggered,
						ActionComp,
						&UCharacterActionComponent::Move
					);
					EnhancedInput->BindAction(PlayerController->MoveAction,
						ETriggerEvent::Completed,
						ActionComp,
						&UCharacterActionComponent::StopMove
					);
				}
				if (PlayerController->JumpAction)
				{
					EnhancedInput->BindAction(PlayerController->JumpAction,
						ETriggerEvent::Triggered,
						ActionComp,
						&UCharacterActionComponent::StartJump
					);
					EnhancedInput->BindAction(PlayerController->JumpAction,
						ETriggerEvent::Completed,
						ActionComp,
						&UCharacterActionComponent::StopJump
					);
				}
				if (PlayerController->LookAction)
				{
					EnhancedInput->BindAction(PlayerController->LookAction,
						ETriggerEvent::Triggered,
						ActionComp,
						&UCharacterActionComponent::Look
					);
				}
				if (PlayerController->SprintAction)
				{
					EnhancedInput->BindAction(PlayerController->SprintAction,
						ETriggerEvent::Triggered,
						ActionComp,
						&UCharacterActionComponent::StartSprint
					);
					EnhancedInput->BindAction(PlayerController->SprintAction,
						ETriggerEvent::Completed,
						ActionComp,
						&UCharacterActionComponent::StopSprint
					);
				}
				if (PlayerController->ShoulderAction)
				{
					EnhancedInput->BindAction(PlayerController->ShoulderAction,
						ETriggerEvent::Triggered,
						ActionComp,
						&UCharacterActionComponent::StartShoulder
					);
					EnhancedInput->BindAction(PlayerController->ShoulderAction,
						ETriggerEvent::Completed,
						ActionComp,
						&UCharacterActionComponent::StopShoulder
					);
				}
				if (PlayerController->ShootingAction)
				{
					EnhancedInput->BindAction(PlayerController->ShootingAction,
						ETriggerEvent::Triggered,
						this,
						&ATaskPlayer::CheckShooting
					);
					EnhancedInput->BindAction(PlayerController->ShootingAction,
						ETriggerEvent::Completed,
						ActionComp,
						&UCharacterActionComponent::StopShooting
					);
				}
				if (PlayerController->GrenadeAction)
				{
					EnhancedInput->BindAction(PlayerController->GrenadeAction,
						ETriggerEvent::Triggered,
						this,
						&ATaskPlayer::Grenade
					);
				}

				//if (PlayerController->ShotAction)
				//{
				//	EnhancedInput->BindAction(PlayerController->ShotAction,
				//		ETriggerEvent::Triggered,
				//		this,
				//		&ATaskPlayer::ShotAction
				//	);
				//}
			}
		}
	}
}

void ATaskPlayer::Attacked(AActor* DamageCauser, float Damage)
{
	float CurrentHealth = Health;
	Super::Attacked(DamageCauser, Damage);

	if (UIngameHUD* nowHud = GetIngameHud())
	{
		nowHud->OnHealthChange(CurrentHealth, Health);
	}

	if (IsDead())
		return;

	ActionComp->Attacked(DamageCauser);
}

void ATaskPlayer::Die()
{
	ActionComp->Die();

}

void ATaskPlayer::GameOver()
{
	if (ATaskPlayerController* PlayerController = Cast<ATaskPlayerController>(GetController()))
	{
		PlayerController->ShowResultHUD();
	}
}

void ATaskPlayer::EquipFirstSlot()
{
	if (EquipComponent)
	{
		if (EquipComponent->Equip(EEquipSlot::First))
		{	
			EquipChange();
		}
	}
}

void ATaskPlayer::EquipSecondSlot()
{
	if (EquipComponent)
	{
		if (EquipComponent->Equip(EEquipSlot::Second))
		{
			EquipChange();
		}
	}
}

void ATaskPlayer::EquipThirdSlot()
{
	if (EquipComponent)
	{
		if (EquipComponent->Equip(EEquipSlot::Third))
		{
			EquipChange();
		}
	}
}

void ATaskPlayer::EquipChange()
{
	AShootWeapon* NewShootWeapon = nullptr;

	if (AWeaponBase* BaseWeapon = EquipComponent->GetCurrentWeapon())
	{
		NewShootWeapon = Cast<AShootWeapon>(BaseWeapon);
	}

	if (!NewShootWeapon)
		return;

	if (NewShootWeapon == NowShootWeapon)
	{
		if (NowShootWeapon)
		{
			NowShootWeapon->OnFireSuccess.AddUniqueDynamic(this, &ATaskPlayer::ShootingAction);
		}
		return;
	}

	if (NowShootWeapon)
	{
		NowShootWeapon->OnFireSuccess.RemoveDynamic(this, &ATaskPlayer::ShootingAction);
		NowShootWeapon = nullptr;
	}

	if (NewShootWeapon)
	{
		NowShootWeapon = NewShootWeapon;
		NowShootWeapon->OnFireSuccess.AddUniqueDynamic(this, &ATaskPlayer::ShootingAction);

		if (ActionComp)
		{
			ActionComp->EquipChange(NowShootWeapon->GetShootType());

			if (UIngameHUD* nowHud = GetIngameHud())
			{
				nowHud->OnGunChanged(NowShootWeapon->GetShootType());
			}
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Current weapon is not AShootWeapon (or none). Unbound."));
	}
}

void ATaskPlayer::CheckShooting()
{
	if (NowShootWeapon)
	{
		if (NowShootWeapon->IsNeedReload())
		{
			ActionComp->DryShot(NowShootWeapon->GetShootType());
		}
		else
		{
			NowShootWeapon->Attack();
		}
	}
}

void ATaskPlayer::ShootingAction()
{
	if (NowShootWeapon && ActionComp)
	{
		ActionComp->Shooting(NowShootWeapon->GetShootType(), ShotSpeedMultiplay);
;	}
}

void ATaskPlayer::Reload()
{
	if (NowShootWeapon && NowShootWeapon->IsFullMagazine() == false && NowShootWeapon->IsReload() == false)
	{
		NowShootWeapon->Reload();
		if (ActionComp)
		{
			ActionComp->Reload(NowShootWeapon->GetShootType(), ReloadSpeedMultiply);
		}
	}
}

void ATaskPlayer::Grenade()
{
	ActionComp->Grenade();
}

void ATaskPlayer::ActiveFieldObject()
{
	AActor* ActiveObject = PickNearestActiveObject();
	if (ActiveObject)
	{
		IInteractInterface::Execute_OnInteract(ActiveObject, this);
		ActionComp->ActiveFuildObject();
	}
}

EShootType ATaskPlayer::GetShootType() const
{
	if (!NowShootWeapon)
		return EShootType::ST_Rifle;

	return NowShootWeapon->GetShootType();
}

void ATaskPlayer::AddPlayerStat(EPlayerShopStat statType, float value)
{
	switch (statType)
	{
	case EPlayerShopStat::MaxHealth: 
		MaxHealth += value;
		Health += value;

		if (ATaskPlayerController* PlayerController = Cast<ATaskPlayerController>(GetController()))
		{
			PlayerController->InitHP(MaxHealth);
		}

		break;
	case EPlayerShopStat::MoveSpeedMultiply:
		MoveSpeedMultiply += value;
		break;
	case EPlayerShopStat::ReloadSpeedMultiply:
		ReloadSpeedMultiply += value;
		break;
	case EPlayerShopStat::ShotSpeedMultiplay:
		ShotSpeedMultiplay += value;
		break;
	default:
		break;
	}
}

void ATaskPlayer::AddActiveObject(AActor* inActiveObject)
{
	if (!IsValid(inActiveObject))
		return;

	if (!inActiveObject->GetClass()->ImplementsInterface(UInteractInterface::StaticClass()))
		return;

	InteractCandidates.AddUnique(inActiveObject);
}

void ATaskPlayer::RemoveActiveObject(AActor* inActiveObject)
{
	InteractCandidates.RemoveAllSwap([inActiveObject](const TWeakObjectPtr<AActor>& W) {
		return !W.IsValid() || W.Get() == inActiveObject;
		});
}

void ATaskPlayer::PruneInvalid()
{
	InteractCandidates.RemoveAllSwap([](const TWeakObjectPtr<AActor>& W) {
		return !W.IsValid();
		});
}

AActor* ATaskPlayer::PickNearestActiveObject()
{
	PruneInvalid();

	AActor* ActiveObject = nullptr;
	float BestDistSqr = TNumericLimits<float>::Max();

	for (const TWeakObjectPtr<AActor>& CheckActiveObject : InteractCandidates)
	{
		if (AActor* Actor = CheckActiveObject.Get())
		{
			const float Distance = FVector::DistSquared(this->GetActorLocation(), Actor->GetActorLocation());
			if (Distance < BestDistSqr)
			{
				BestDistSqr = Distance;
				ActiveObject = Actor;
			}
		}
	}
	return ActiveObject;
}

UIngameHUD* ATaskPlayer::GetIngameHud()
{
	if (UWorld* World = GetWorld())
	{
		if (AZWaveGameState* ZGS = Cast<AZWaveGameState>(World->GetGameState()))
		{
			return ZGS->IngameHUD;
		}
	}

	return nullptr;
}

void ATaskPlayer::SetHealth(float SetHealAmount)
{
	float CurrentHealth = Health;
	Super::SetHealth(SetHealAmount);

	if (UIngameHUD* nowHud = GetIngameHud())
	{
		nowHud->OnHealthChange(CurrentHealth, Health);
	}
}