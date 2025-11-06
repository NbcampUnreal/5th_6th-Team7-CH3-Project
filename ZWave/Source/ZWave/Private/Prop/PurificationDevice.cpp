// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PurificationDevice.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "Effect/EffectApplyManager.h"
#include "DamageCalculator/DamageCalculator.h"

// Sets default values
APurificationDevice::APurificationDevice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	ExplodeLocation = CreateDefaultSubobject<USceneComponent>(TEXT("ExplodeLocation"));
	ExplodeLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APurificationDevice::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
}

float APurificationDevice::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (const FZWaveDamageEvent* CustomDamageEvent = static_cast<const FZWaveDamageEvent*>(&DamageEvent))
	{
		if (UEffectApplyManager* EffectManager = GetWorld()->GetSubsystem<UEffectApplyManager>())
		{
			EffectManager->ApplyEffect(this, CustomDamageEvent->EffectArray, CustomDamageEvent->Duration);
		}
		

		Attacked(DamageCauser, DamageAmount); //데미지 계산 후 넘겨줄 수도 있고 아니면 그냥 이렇게 쓸 수도 있을 듯
	}

	return DamageAmount;
}

void APurificationDevice::Attacked(AActor* DamageCauser, float Damage)
{
	ApplyDamage(Damage);
}

void APurificationDevice::ApplyDamage(float Damage, bool CheckArmor)
{
	Health -= Damage;
	if (Health <= 0.f)
	{
		Die();
	}
}

void APurificationDevice::Die()
{
	if (bIsEnd == false)
	{
		bIsEnd = true;
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Niagara_Explode, ExplodeLocation->GetComponentLocation(), FRotator::ZeroRotator);

	}
}

