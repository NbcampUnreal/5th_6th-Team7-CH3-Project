// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/ClimbingEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/ClimbingEnemy.h"
#include "Level/ClimbingPoint.h"
#include "Level/CeilingPoint.h"
#include "Enemy/ClimbingAIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetMathLibrary.h>

AClimbingEnemy::AClimbingEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	ClimbingSpeed = 300.f;
	EndCeiling = 300.f;
	State = EClimbingState::None;
}

void AClimbingEnemy::BeginPlay()
{
	Super::BeginPlay();

	Target = UGameplayStatics::GetPlayerCharacter(this, 0);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AClimbingEnemy::OnCapsuleBeginOverlap);
	GetCharacterMovement()->MaxFlySpeed = ClimbingSpeed;
}

void AClimbingEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (State)
	{
	case EClimbingState::Ceiling:
	{
		FVector MyLocation = this->GetActorLocation();
		FVector DestLocation = Target->GetActorLocation();
		DestLocation.Z = MyLocation.Z;

		AddMovementInput((DestLocation - MyLocation).GetSafeNormal(), 1.f);
		const bool CheckEndCeiling = FVector::Dist2D(MyLocation, DestLocation) <= EndCeiling;
		const FRotator BaseRot(180.f, 0.f, 0.f);
		FVector To = DestLocation - GetActorLocation();
		To.Z = 0.f;
		FRotator NewRot;
		NewRot.Pitch = 180.f;
		NewRot.Yaw = To.Rotation().Yaw + 90.f;
		NewRot.Roll = 0.f;
		SetActorRotation(NewRot);

		if (CheckEndCeiling)
		{
			AClimbingAIController* AI = Cast<AClimbingAIController>(GetController());
			if (!AI) return;

			NewRot.Pitch = To.Rotation().Pitch;
			NewRot.Yaw = To.Rotation().Yaw + 90.f;
			NewRot.Roll = To.Rotation().Roll;

			SetActorRotation(NewRot);
			GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

			AI->StopClimbing();
			State = EClimbingState::None;
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
			GetCharacterMovement()->GravityScale = 1.f;
		}
	}
		return;
	case EClimbingState::Wall:
		AddMovementInput(FVector::UpVector, 1.f);
		return;
	default:
		return;
	}
}

void AClimbingEnemy::Die()
{
	Super::Die();
}

void AClimbingEnemy::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherComp) return;

	if (OtherActor->IsA<AClimbingPoint>())
	{
		if (State != EClimbingState::None) return;
		AClimbingAIController* AI = Cast<AClimbingAIController>(GetController());
		if (!AI) return;

		State = EClimbingState::Wall;
		GetCharacterMovement()->MaxFlySpeed = ClimbingSpeed;
		SetActorRotation(OtherActor->GetActorRotation());
		GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, -90.f));

		if (UPathFollowingComponent* PF = AI->GetPathFollowingComponent())
			PF->AbortMove(*AI, FPathFollowingResultFlags::ForcedScript);

		AI->StopMovement();
		AI->StartClimbing();
		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->GravityScale = 0.f;
	}
	if (OtherActor->IsA<ACeilingPoint>())
	{
		if (State != EClimbingState::Wall) return;
		State = EClimbingState::Ceiling;

		GetMesh()->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		FRotator Now = GetActorRotation();
		Now.Pitch = 180.f;
		SetActorRotation(Now);
	}
}
