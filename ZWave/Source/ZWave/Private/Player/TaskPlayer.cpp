// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TaskPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"   
#include "GameFramework/SpringArmComponent.h"
#include "Player/CharacterActionComponent.h" 
#include "EnhancedInputComponent.h"
#include "Player/TaskPlayerController.h"

ATaskPlayer::ATaskPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	ActionComp = CreateDefaultSubobject<UCharacterActionComponent>(TEXT("ActionComp"));

	GetCharacterMovement()->MaxWalkSpeed = 300.f;
}

void ATaskPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	if (ActionComp)
	{
		ActionComp->InitRefs(GetCharacterMovement(), SpringArmComp, CameraComp);
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
			if (PlayerController->ShotAction)
			{
				EnhancedInput->BindAction(PlayerController->ShotAction,
					ETriggerEvent::Triggered,
					ActionComp,
					&UCharacterActionComponent::Shot
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
		}
	}
}

