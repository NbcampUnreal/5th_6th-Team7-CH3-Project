// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "GenericTeamAgentInterface.h"
#include "AoEActor.generated.h"

UCLASS()
class ZWAVE_API AAoEActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAoEActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class UBoxComponent* BoxCollision;
	class UNiagaraComponent* NiagaraParticleInstance;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundWave* EffectSound = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "SFX")
	UAudioComponent* EffectSoundAC = nullptr;

	UPROPERTY(EditAnywhere, Category = "SFX")
	USoundAttenuation* EffectSoundAttenuation = nullptr;

public:
	UFUNCTION(BlueprintCallable, Category = "AoE")
	void ActiveAoE(UNiagaraSystem* NiagaraParticle, struct FAoEParam DamageParam);

	UFUNCTION(BlueprintCallable, Category = "AoEDoT")
	void ApplyOverlapActorDOT();

	UFUNCTION(BlueprintCallable, Category = "AoE")
	void ApplyOverlapActorDamage();

	UFUNCTION(BlueprintCallable, Category = "AoE")
	void DestroyAoEActor(UNiagaraComponent* NiagaraSys);

private:
	FTimerHandle DOTHandle;
	float DamagePerSecond;
	float TotalActiveTime;
	float CurrentActiveTime;

	TSubclassOf<class UEffectBase> AoEEffectClass;
};
