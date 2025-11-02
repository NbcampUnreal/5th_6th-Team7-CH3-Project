// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BombZombie.h"

#include "Base/BaseCharacter.h"

void ABombZombie::Attack()
{
	Super::Attack();

	TArray<FHitResult> HitResults;
	float SphereRadius = this->AttackRange * 2;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility,                  // 사용할 충돌 채널 설정
		FCollisionShape::MakeSphere(SphereRadius)
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (Hit.GetActor()->IsA(ABaseCharacter::StaticClass()))
			{
				ABaseCharacter* Target = static_cast<ABaseCharacter*>(Hit.GetActor());

				if (Target->GetHealth() > 0)
				{
					UE_LOG(LogTemp, Display, TEXT("Target name: %s"), *Target->GetActorNameOrLabel());
					Attack();
					GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ATurret::Attack, FireInterval, true);
					break;
				}
			}
		}
	}
}
