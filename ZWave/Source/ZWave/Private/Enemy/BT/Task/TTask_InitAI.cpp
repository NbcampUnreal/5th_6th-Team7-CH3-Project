// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BT/Task/TTask_InitAI.h"

#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Base/ZWaveGameMode.h"
#include "Prop/PurificationDevice.h"

UTTask_InitAI::UTTask_InitAI()
{
	NodeName = TEXT("InitAI");
}


EBTNodeResult::Type UTTask_InitAI::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* OwnerBlackboard = OwnerComp.GetBlackboardComponent();
	if (OwnerBlackboard == nullptr) return EBTNodeResult::Failed;

	if (OwnerBlackboard->IsVectorValueSet(GetSelectedBlackboardKey()) == false)
	{
		AZWaveGameMode* GameMode = static_cast<AZWaveGameMode*>(UGameplayStatics::GetGameMode(this));
		if (GameMode == nullptr) return EBTNodeResult::Failed;
		if (GameMode->PuriDevice == nullptr) return EBTNodeResult::Failed;

		OwnerBlackboard->SetValueAsVector(GetSelectedBlackboardKey(), GameMode->PuriDevice->GetActorLocation());
	}

	return EBTNodeResult::Succeeded;
}
