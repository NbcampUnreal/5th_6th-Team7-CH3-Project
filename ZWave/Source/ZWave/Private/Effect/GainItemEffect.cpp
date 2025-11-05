// Fill out your copyright notice in the Description page of Project Settings.
#include "Effect/GainItemEffect.h"

UGainItemEffect::UGainItemEffect()
{

}

void UGainItemEffect::Init()
{

}

void UGainItemEffect::ApplyEffect(AActor* TargetActor, const float& Duration)
{
	
}

void UGainItemEffect::ApplyEffect(AActor* TargetActor, const int32& Itemindex)
{
	if (ATaskPlayer* Player = Cast<ATaskPlayer>(TargetActor))
	{
		// TODO:
		// 인덱스에 해당하는 아이템을 인벤토리에 넣어줌

		RemoveEffect();
	}
}

void UGainItemEffect::RemoveEffect()
{
	Super::RemoveEffect();
}

void UGainItemEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UGainItemEffect has been successfully collected by GC."));
}


