// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FindDest.generated.h"

/**
 * 
 */
UCLASS()
class ZWAVE_API UBTService_FindDest : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_FindDest();

private:
	UPROPERTY(EditAnywhere)
	float SightRange = 2000;
	UPROPERTY(EditAnywhere)
	float AutoDetectionRange = 200;
	UPROPERTY(EditAnywhere)
	float SightDegree = 180;    
	UPROPERTY(EditAnywhere)
	float ReachRadius = 150.f;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void TickWithIsAggroedCondtion(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	void TickWithIsNotAggroedCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

};
