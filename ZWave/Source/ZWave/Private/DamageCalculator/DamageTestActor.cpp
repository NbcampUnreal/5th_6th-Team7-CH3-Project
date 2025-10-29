// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageCalculator/DamageTestActor.h"
#include "Kismet/GameplayStatics.h"
#include "Effect/StaggerEffect.h"
#include "Effect/EffectApplyManager.h"

// Sets default values
ADamageTestActor::ADamageTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADamageTestActor::BeginPlay()
{
	Super::BeginPlay();

	FCustomDamageEvent CustomEvent;

	CustomEvent.EffectArray.Add(UStaggerEffect::StaticClass());
	TakeDamage(0, CustomEvent, GetInstigatorController(), this);
}

float ADamageTestActor::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	if (DamageEvent.IsOfType(FDamageEvent::ClassID))
	{
		if (const FCustomDamageEvent* CustomDamageEvent = static_cast<const FCustomDamageEvent*>(&DamageEvent))
		{

			UE_LOG(LogTemp, Warning, TEXT("Damage Event Log"));
			//TArray<TSubclassOf<UEffectBase>> EffectArr = CustomDamageEvent->EffectArray;
			//UEffectApplyManager* Manager = GetWorld()->GetSubsystem<UEffectApplyManager>();
			//Manager->ApplyEffect(nullptr, 0, 0, EffectArr);
		}
	}
	
	return 0.0f;
}

