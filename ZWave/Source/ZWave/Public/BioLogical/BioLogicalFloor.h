// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Base/Damagable.h"
#include "GenericTeamAgentInterface.h"
#include "BioLogicalFloor.generated.h"

UCLASS()
class ZWAVE_API ABioLogicalFloor : public AActor, public IDamagable, public IGenericTeamAgentInterface
{
	GENERATED_BODY()
	
public:	
	ABioLogicalFloor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

public:
	// 인터페이스 구현부
	virtual void Attacked(AActor* DamageCauser, float Damage);
	virtual void ApplyDamage(float Damage, bool CheckArmor = true);
	virtual void Die();
	virtual FGenericTeamId GetGenericTeamId() const override;
	//
	
	UFUNCTION(BlueprintCallable, Category = "Generate")
	void ReGenerateFloor();


	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float FloorHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxFloorHealth;

	UPROPERTY(EditDefaultsOnly, Category = "GenerateTime")
	float GenerateTime;

private:
	FTimerHandle ReGenerateTimer;
	FTransform OriginalTransform;
};
