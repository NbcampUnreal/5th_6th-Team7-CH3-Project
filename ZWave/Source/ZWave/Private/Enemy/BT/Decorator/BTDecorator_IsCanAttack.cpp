// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Decorator/BTDecorator_IsCanAttack.h"


#include "Enemy/BaseEnemy.h"
#include "Enemy/BaseAIController.h"
#include "Weapon/EquipComponent.h"
#include "Weapon/WeaponBase.h"

UBTDecorator_IsCanAttack::UBTDecorator_IsCanAttack()
{
	NodeName = FString(TEXT("IsCanAttack?"));
}

bool UBTDecorator_IsCanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	ABaseAIController* AIController = static_cast<ABaseAIController*>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) return false;

	ABaseEnemy* AICharacter = static_cast<ABaseEnemy*>(AIController->GetCharacter());
	if (AICharacter == nullptr) return false;

	UEquipComponent* EquipComp = AICharacter->FindComponentByClass<UEquipComponent>();
	if (EquipComp)
	{
		if (AWeaponBase* WeaponActor = EquipComp->GetCurrentWeapon())
		{
			if (WeaponActor->CanAttack()) 
			{
				return true;
			}
		}
	}
	return false;
}


