// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Decorator/BTDecorator_IsCanRangedAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/BaseAIController.h"
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

	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr) return false;

	ABaseEnemy* EnemyCharacter = Cast<ABaseEnemy>(AIController->GetCharacter());
	if (EnemyCharacter == nullptr) return false;

	AActor* TargetActor = Cast<AActor>(OwnerBlackboard->GetValueAsObject(FName(TEXT("SecondaryTarget"))));
	if (TargetActor == nullptr)
		return false;

	UEquipComponent* EquipComponent = EnemyCharacter->FindComponentByClass<UEquipComponent>();
	if (EquipComponent == nullptr) return false;

	AWeaponBase* WeaponActor = EquipComponent->GetCurrentWeapon();
	if (WeaponActor)
	{
		if (!WeaponActor->CanAttack())
		{
			return false;
		}
	}
	else
		return false;

	UWorld* World = EnemyCharacter->GetWorld();
	if (World == nullptr)
		return false;

	const FVector Start = EnemyCharacter->GetActorLocation() + FVector(0.f, 0.f, 50.f); // 살짝 위에서 시작
	const FVector End = TargetActor->GetActorLocation() + FVector(0.f, 0.f, 50.f);

	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(AI_SightTrace), true, EnemyCharacter);
	TraceParams.bReturnPhysicalMaterial = false;

	FHitResult HitResult;
	const bool bHit = World->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECollisionChannel::ECC_GameTraceChannel1,
		TraceParams
	);

	if (bHit)
	{
		if (HitResult.GetActor() == TargetActor)
		{
			return true;
		}

		return false;
	}

	return false;
}
