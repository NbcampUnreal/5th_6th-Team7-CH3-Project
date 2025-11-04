// Fill out your copyright notice in the Description page of Project Settings.
#include "AoE/AoEActor.h"
#include "Components/BoxComponent.h"
#include "Base/BaseCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "AoE/AoEData.h"
#include "DamageCalculator/DamageCalculator.h"

AAoEActor::AAoEActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(Scene);
	BoxCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	DamagePerSecond = 0;
	TotalActiveTime = 0;
	CurrentActiveTime = 0;
	AoEEffectClass = nullptr;
}

void AAoEActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAoEActor::ActiveAoE(UNiagaraSystem* NiagaraParticle, FAoEParam DamageParam)
{
	this->DamagePerSecond = DamageParam.DamagePerSec;
	this->TotalActiveTime = DamageParam.ActiveTime;
	this->AoEEffectClass = DamageParam.AoEEffectClass;
	if (!NiagaraParticle)
	{
		return;
	}

	BoxCollision->SetBoxExtent(DamageParam.AoERange);

	NiagaraParticleInstance = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		NiagaraParticle,
		GetActorLocation()
	);

	if (NiagaraParticleInstance)
	{
		FVector BoxSize = BoxCollision->GetScaledBoxExtent() * 2.0f; // 실제 풀 사이즈
		NiagaraParticleInstance->SetWorldScale3D(BoxSize / 100.f);
		NiagaraParticleInstance->OnSystemFinished.AddDynamic(this, &AAoEActor::DestroyAoEActor);
	}

	if (!DamageParam.bIsTick)
	{
		ApplyOverlapActorDamage();
	}
	else
	{
		GetWorldTimerManager().SetTimer(
			DOTHandle,
			this,
			&AAoEActor::ApplyOverlapActorDOT,
			1.0f,
			true
		);
	}
}

void AAoEActor::ApplyOverlapActorDOT()
{
	if (CurrentActiveTime > TotalActiveTime)
	{
		GetWorldTimerManager().ClearTimer(DOTHandle);
		NiagaraParticleInstance->Deactivate();
	}
	else
	{
		CurrentActiveTime += 1.0f;

		TArray<AActor*> OverlapActors;
		BoxCollision->GetOverlappingActors(OverlapActors, ABaseCharacter::StaticClass());

		for (AActor* OverlapActor : OverlapActors)
		{
			if (!OverlapActor && OverlapActor == GetOwner())
			{
				continue;
			}

			FZWaveDamageEvent DamageEvent;
			DamageEvent.BaseDamage = DamagePerSecond;
			DamageEvent.Duration = TotalActiveTime;
			if (AoEEffectClass)
			{
				DamageEvent.EffectArray.Add(AoEEffectClass);
			}
			UDamageCalculator::DamageHelper(GetWorld(), OverlapActor, this, DamageEvent);
		}
	}
}

void AAoEActor::ApplyOverlapActorDamage()
{
	TArray<AActor*> OverlapActors;
	BoxCollision->GetOverlappingActors(OverlapActors, ABaseCharacter::StaticClass());

	for (AActor* OverlapActor : OverlapActors)
	{
		if (!OverlapActor && OverlapActor == GetOwner())
		{
			continue;
		}

		FZWaveDamageEvent DamageEvent;
		DamageEvent.BaseDamage = DamagePerSecond;

		UDamageCalculator::DamageHelper(GetWorld(), OverlapActor, this, DamageEvent);
	}
}


void AAoEActor::DestroyAoEActor(UNiagaraComponent* NiagaraSys)
{
	Destroy();
}



