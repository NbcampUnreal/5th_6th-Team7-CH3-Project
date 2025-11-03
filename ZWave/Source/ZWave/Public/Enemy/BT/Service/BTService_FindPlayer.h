// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "InputCoreTypes.h"

#include "BTService_FindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UBTService_FindPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_FindPlayer();

private:
	UPROPERTY(EditAnywhere)
	float SightRange = 2000;
	UPROPERTY(EditAnywhere)
	float AutoDetectionRange = 200;
	UPROPERTY(EditAnywhere)
	float SightDegree = 180;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void TickWithIsAggroedCondtion(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds, class ABaseAIController* MyController, class ABaseEnemy* MyCharacter);
	void TickWithIsNotAggroedCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds, class ABaseAIController* MyController, class ABaseEnemy* MyCharacter);

	bool CheckSightRange(FVector ToTargetVector, float sightRange);
	bool CheckSightDegree(AActor* StdActor, FVector ToTargetVector, bool bIsSecondaryTargetLocationSet);

	void ClearTarget(class UBlackboardComponent* OwnerBlackboard);
};
