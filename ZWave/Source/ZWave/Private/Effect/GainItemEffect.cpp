// Fill out your copyright notice in the Description page of Project Settings.
#include "Effect/GainItemEffect.h"

UGainItemEffect::UGainItemEffect()
{

}

void UGainItemEffect::ApplyEffect(AActor* TargetActor, const float& BaseDamage)
{
	if (ATaskPlayer* Player = Cast<ATaskPlayer>(TargetActor))
	{
		// TODO:
		// 인벤토리 추가 되면 랜덤 확률로 아이템 정해주고, 해당 아이템 인벤토리에 추가 후 삭제
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
