// Fill out your copyright notice in the Description page of Project Settings.

#include "Effect/ReloadEffect.h"
#include "Weapon/EquipComponent.h"

UReloadEffect::UReloadEffect()
{
	ReloadSpeedPer = 0.3f;
}

void UReloadEffect::ApplyEffect(AActor* TargetActor, const float& BaseDamage)
{
	if (ABaseCharacter* Player = Cast<ABaseCharacter>(TargetActor))
	{
		if(UEquipComponent* EquipComponent = Player->GetComponentByClass<UEquipComponent>())
		{
			//재장전 속도 변경 로직 작성
		}
	}
}

void UReloadEffect::RemoveEffect()
{
	Super::RemoveEffect();
}

void UReloadEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UReloadEffect has been successfully Collected GC."));
}


