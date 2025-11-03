#include "Effect/HealthEffect.h"
#include "Player/TaskPlayer.h"

UHealthEffect::UHealthEffect()
{
	HealAmount = 0.3f;
}

void UHealthEffect::ApplyEffect(AActor* TargetActor, const float& Duration)
{
	if (ABaseCharacter* Player = Cast<ABaseCharacter>(TargetActor))
	{
		// TODO :
		// 해당 부분에서 Player 체력 올려주는 로직 작성
		float ChangeHealth = Player->GetMaxHealth() * HealAmount;
		Player->SetHealth(ChangeHealth);
		RemoveEffect();
	}
}

void UHealthEffect::RemoveEffect()
{
	Super::RemoveEffect();
}

void UHealthEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UHealthEffect has been successfully collected by GC."));
}


