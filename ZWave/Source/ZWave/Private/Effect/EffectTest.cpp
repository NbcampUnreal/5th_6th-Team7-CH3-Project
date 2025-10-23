// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/EffectTest.h"
#include "Effect/EffectApplyManager.h"
#include "Engine/World.h"
#include "Effect/StaggerEffect.h"
#include "Mode/ModeDefinition.h"

AEffectTest::AEffectTest()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM"));
	SM->SetupAttachment(Scene);
}


void AEffectTest::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(
		TestHandle,
		this,
		&AEffectTest::Move,
		0.1f,
		true
	);

	GetWorld()->GetTimerManager().SetTimer(
		EffectHandle,
		this,
		&AEffectTest::TestEffect,
		3.0f,
		true
	);
}


void AEffectTest::Move()
{
	FVector CurVec = GetActorLocation();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	CurVec.X += Speed * DeltaTime;
	SetActorLocation(CurVec);
}

void AEffectTest::TestEffect()
{
	UEffectApplyManager* Manager = GetWorld()->GetSubsystem<UEffectApplyManager>();
	Manager->ApplyEffect(this, ModeData->EffectStruct.EffectValue, ModeData->EffectClass);
}



