// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Base/Damagable.h"
#include "GenericTeamAgentInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ZWAVE_API ABaseCharacter : public ACharacter, public IDamagable, public IGenericTeamAgentInterface
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
	
	// 데미지 및 아이템 쪽에서 사용하기 위해 추가되었습니다.
	virtual float GetMaxHealth() const;
	virtual void SetHealth(float SetHealAmount);
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//

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

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID;

public:
	float GetHealth() const;

};
