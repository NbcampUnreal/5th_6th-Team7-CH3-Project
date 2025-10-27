// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PurificationDevice.h"

// Sets default values
APurificationDevice::APurificationDevice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APurificationDevice::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APurificationDevice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APurificationDevice::Attacked(AActor* DamageCauser, float Damage)
{
}

void APurificationDevice::ApplyDamage(float Damage, bool CheckArmor)
{
}

void APurificationDevice::Die()
{
}

