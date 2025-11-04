// Fill out your copyright notice in the Description page of Project Settings.


#include "State/StateComponent.h"

// Sets default values for this component's properties
UStateComponent::UStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	CurrentState = EStateType::EST_None;
}


// Called when the game starts
void UStateComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UStateComponent::SetState(EStateType SetType)
{
	CurrentState = SetType;
}


