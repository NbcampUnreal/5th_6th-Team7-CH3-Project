// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base/Damagable.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ZWAVE_API ABaseCharacter : public ACharacter, public IDamagable 
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Attacked(AActor* DamageCauser, float Damage) override;
	virtual void ApplyDamage(float Damage, bool CheckArmor = true) override;
	virtual float GetMaxHealth();
	virtual void SetHealth(float SetHealAmount);

	virtual void Die() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component|Equip")
	TObjectPtr<class UEquipComponent> EquipComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Stat")
	float MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Stat")
	float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Stat")
	float Armor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Stat")
	float SpeedMultiply;

};
