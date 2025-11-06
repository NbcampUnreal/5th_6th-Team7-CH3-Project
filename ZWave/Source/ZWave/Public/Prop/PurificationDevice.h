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

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = 0;

public:
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Attacked(AActor* DamageCauser, float Damage) override;
	virtual void ApplyDamage(float Damage, bool CheckArmor = true) override;
	virtual void Die() override;

};
