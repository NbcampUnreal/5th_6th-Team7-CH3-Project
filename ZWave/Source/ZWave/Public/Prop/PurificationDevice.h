// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GenericTeamAgentInterface.h"

#include "Base/Damagable.h"

#include "PurificationDevice.generated.h"

UCLASS()
class ZWAVE_API APurificationDevice : public AActor, public IDamagable, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APurificationDevice();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> ExplodeLocation;
	
	class UIngameHUD* GetIngameHud();

/// <summary>
/// 피격 관련
/// </summary>
protected:
	UPROPERTY(EditAnywhere, Category = "Team")
	uint8 TeamID = 1;
	virtual FGenericTeamId GetGenericTeamId() const override;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float MaxHealth;
	UPROPERTY(VisibleAnywhere)
	float Health;
	
	bool bIsEnd = false;
	FTimerHandle GameOverTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<class USoundBase> ExplodeSound;
	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<class UNiagaraSystem> Niagara_Explode;


public:
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Attacked(AActor* DamageCauser, float Damage) override;
	virtual void ApplyDamage(float Damage, bool CheckArmor = true) override;
	virtual void Die() override;

	void CallGameOver();
};
