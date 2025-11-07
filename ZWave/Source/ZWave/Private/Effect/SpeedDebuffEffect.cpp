// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/SpeedDebuffEffect.h"

USpeedDebuffEffect::USpeedDebuffEffect()
{
	SpeedMultiplier = 0.3f;
}

void USpeedDebuffEffect::ApplyEffect(AActor* TargetActor, const float& Duration)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(TargetActor))
	{
		Target = Character;
		const float CharacterSpeed = Character->GetMoveSpeed();
		Character->SetMoveSpeed(CharacterSpeed - (CharacterSpeed * SpeedMultiplier));

		if (!FMath::IsNearlyZero(Duration))
		{
			SetBuffTimer(Duration);
		}
		else
		{
			RemoveEffect();
		}
	}
}

void USpeedDebuffEffect::RemoveEffect()
{
	// 이속 증가/감소 효과 제거 로직 작성
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(Target))
	{
		const float CharacterSpeed = Character->GetMoveSpeed();
		Character->SetMoveSpeed(CharacterSpeed + (CharacterSpeed * SpeedMultiplier));
	}
	Super::RemoveEffect();
}

void USpeedDebuffEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("USpeedDebuffEffect has been successfully collected by GC."));
}

void USpeedDebuffEffect::SetBuffTimer(const float& Duration)
{
	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			World->GetTimerManager().SetTimer(
				BuffTimer,
				this,
				&USpeedDebuffEffect::RemoveEffect,
				Duration,
				false
			);
		}
	}
}
