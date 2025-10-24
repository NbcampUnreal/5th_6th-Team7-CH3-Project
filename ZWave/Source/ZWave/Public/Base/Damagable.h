// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damagable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamagable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZWAVE_API IDamagable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//이후 필요하다면 Damage를 Bullet 이나, AttackInfo  Struct로 만들어서 경직 시간, 데미지, 디버프 정보 등을 넣어서 보내주는 식으로 확장하는건 어떨까요?
	virtual void Attacked(AActor* DamageCauser, float Damage) = 0;
	
	virtual void ApplyDamage(float Damage, bool CheckArmor = true) = 0;
	virtual void Die() = 0;
};
