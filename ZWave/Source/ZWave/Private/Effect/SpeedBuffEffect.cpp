// Fill out your copyright notice in the Description page of Project Settings.

#include "Effect/SpeedBuffEffect.h"
#include "Player/CharacterActionComponent.h"


USpeedBuffEffect::USpeedBuffEffect()
{
	SpeedMultiplier = 1.5f;
}

void USpeedBuffEffect::ApplyEffect(AActor* TargetActor, const float& Duration)
{
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(TargetActor))
	{
		if (!FMath::IsNearlyZero(Duration))
		{
			SetBuffTimer(Duration);
		}
		Target = Character;

		if (UCharacterActionComponent* ActionComp = Character->FindComponentByClass<UCharacterActionComponent>())
		{
			ActionComp->ChangeSpeedMultiply(SpeedMultiplier);
		}
		else
		{
			const float CharacterSpeed = Character->GetMoveSpeed();
			Character->SetMoveSpeed(CharacterSpeed * SpeedMultiplier);
		}
	}
}

void USpeedBuffEffect::RemoveEffect()
{
	// 이속 증가/감소 효과 제거 로직 작성
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(Target))
	{
		if (UCharacterActionComponent* ActionComp = Character->FindComponentByClass<UCharacterActionComponent>())
		{
			ActionComp->ChangeSpeedMultiply();
		}
		else
		{
			const float CharacterSpeed = Character->GetMoveSpeed();
			const float RemoveSpeedMultiplier = SpeedMultiplier - 1.0f;
			Character->SetMoveSpeed(CharacterSpeed - (CharacterSpeed * RemoveSpeedMultiplier));
		}
	}
	
	Super::RemoveEffect();
}

void USpeedBuffEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("USpeedBuffEffect has been successfully collected by GC."));
}

void USpeedBuffEffect::SetBuffTimer(const float& Duration)
{
	if (UObject* Outer = GetOuter())
	{
		if (UWorld* World = Outer->GetWorld())
		{
			World->GetTimerManager().SetTimer(
				BuffTimer,
				this,
				&USpeedBuffEffect::RemoveEffect,
				Duration,
				false
			);
		}
	}
}

