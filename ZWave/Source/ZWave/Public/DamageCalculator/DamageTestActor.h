// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DamageEvents.h"
#include "GenericTeamAgentInterface.h"
#include "DamageTestActor.generated.h"

USTRUCT(BlueprintType)
struct FCustomDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FCustomDamageEvent() { DamageTypeClass = UDamageType::StaticClass(); }

	TArray<TSubclassOf<class UEffectBase>> EffectArray;
};

UCLASS()
class ZWAVE_API ADamageTestActor : public AActor, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual FORCEINLINE FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamId); }

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamId; //0 ~ 255 255 중립 

};
