// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/CeilingPoint.h"
#include "Components/BoxComponent.h"

// Sets default values
ACeilingPoint::ACeilingPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    BoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("CapsuleRoot"));
    SetRootComponent(BoxRoot);
}


