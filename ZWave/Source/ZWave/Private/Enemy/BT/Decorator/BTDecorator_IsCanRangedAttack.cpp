// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Decorator/BTDecorator_IsCanRangedAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/RangedAIController.h"
#include "Weapon/EquipComponent.h"
#include "Weapon/WeaponBase.h"

UBTDecorator_IsCanRangedAttack::UBTDecorator_IsCanRangedAttack()
{
	NodeName = FString(TEXT("IsCanRangedAttack?"));
}

bool UBTDecorator_IsCanRangedAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return false;

	ARangedAIController* AIController = Cast<ARangedAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) return false;

	ABaseEnemy* EnemyCharacter = Cast<ABaseEnemy>(AIController->GetCharacter());
	if (EnemyCharacter == nullptr) return false;

	AActor* TargetActor = Cast<AActor>(OwnerBlackboard->GetValueAsObject(FName(TEXT("SecondaryTarget"))));
	if (TargetActor == nullptr)
		return false;

	if (!AIController->CheckCondition(TargetActor))
		return false;

	return true;
}
