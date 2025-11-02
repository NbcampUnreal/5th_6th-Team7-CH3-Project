// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BombZombie.h"

#include "Base/BaseCharacter.h"

void ABombZombie::Die()
{
	//Bomb();

	Super::Die();
}

void ABombZombie::Attack()
{
	Super::Attack();
	UE_LOG(LogTemp, Display, TEXT("Bomb attack"));

	//Attacked(this, this->MaxHealth);
}

void ABombZombie::Bomb()
{
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
				//Target->Attacked()
			}
		}
	}
}
