// Fill out your copyright notice in the Description page of Project Settings.


#include "BioLogical/BioLogicalFloor.h"

// Sets default values
ABioLogicalFloor::ABioLogicalFloor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);


	MaxFloorHealth = 100.0f;
	FloorHealth = MaxFloorHealth;
	GenerateTime = 10.0f;
	TeamID = 255;
}

void ABioLogicalFloor::BeginPlay()
{
	FloorHealth = MaxFloorHealth;
}

void ABioLogicalFloor::Attacked(AActor* DamageCauser, float Damage)
{
	ApplyDamage(Damage);
}

void ABioLogicalFloor::ApplyDamage(float Damage, bool CheckArmor)
{
	FloorHealth = FMath::Max(0.f, FloorHealth - Damage);

	if (FloorHealth <= 0.f)
	{
		Die();
	}
}

void ABioLogicalFloor::Die()
{

	GetWorldTimerManager().SetTimer(
		ReGenerateTimer,
		this,
		&ABioLogicalFloor::ReGenerateFloor,
		GenerateTime,
		false
	);


	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

FGenericTeamId ABioLogicalFloor::GetGenericTeamId() const
{
	return TeamID;
}

void ABioLogicalFloor::ReGenerateFloor()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	FloorHealth = MaxFloorHealth;
}

