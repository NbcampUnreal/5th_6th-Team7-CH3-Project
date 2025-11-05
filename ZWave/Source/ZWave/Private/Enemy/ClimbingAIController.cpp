// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ClimbingAIController.h"
#include "Enemy/ClimbingEnemy.h"
#include "Base/ZWaveGameState.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Level/ClimbingPoint.h"

AClimbingAIController::AClimbingAIController()
{
    TargetClimbingPoint = nullptr;
}

bool AClimbingAIController::CheckCondition(AActor* Target)
{
    AClimbingEnemy* MyCharacter = Cast<AClimbingEnemy>(GetCharacter());
    if (!MyCharacter) return false;

    if (MyCharacter->GetClimbingState() != EClimbingState::None) return true;

    UBlackboardComponent* OwnerBlackboardComp = GetBlackboardComponent();
    if (OwnerBlackboardComp == nullptr) return false;

    AZWaveGameState* GameState = GetWorld()->GetGameState<AZWaveGameState>();
    if (GameState == nullptr) return false;

    if (TargetClimbingPoint == nullptr)
    {
        TArray<AClimbingPoint*> ClimbingPoints = GameState->GetClimbingPointArray();
        if (ClimbingPoints.Num() == 0) return false;

        float BestDistSq = TNumericLimits<float>::Max();
        const FVector MyLoc = MyCharacter->GetActorLocation();
        AClimbingPoint* Nearest = nullptr;

        for (AClimbingPoint* ClimbingPoint : ClimbingPoints)
        {
            if (!IsValid(ClimbingPoint)) continue;

            const float DistSq = FVector::DistSquared(MyLoc, ClimbingPoint->GetActorLocation());

            if (DistSq < BestDistSq)
            {
                BestDistSq = DistSq;
                Nearest = ClimbingPoint;
            }
        }

        if (!Nearest) return false;

        const float ToTargetDist = FVector::DistSquared(MyLoc, Target->GetActorLocation());

        if (ToTargetDist < BestDistSq)
            return false;

        TargetClimbingPoint = Nearest; 

        OwnerBlackboardComp->SetValueAsVector(FName(TEXT("AttackLocation")), GetAttackLocation(TargetClimbingPoint->GetActorLocation()));

    }
    else
    {
        const float ToTargetDist = FVector::DistSquared(MyCharacter->GetActorLocation(), Target->GetActorLocation());
        const float ClimbingDist = FVector::DistSquared(MyCharacter->GetActorLocation(), TargetClimbingPoint->GetActorLocation());

        if (ToTargetDist < ClimbingDist)
        {
            ResetClimbingPoint();
            return false;
        }
    }
    return TargetClimbingPoint != nullptr;
}

void AClimbingAIController::ResetClimbingPoint()
{
    UBlackboardComponent* OwnerBlackboardComp = GetBlackboardComponent();
    if (OwnerBlackboardComp == nullptr) return;

    OwnerBlackboardComp->ClearValue(FName(TEXT("AttackLocation")));
    TargetClimbingPoint = nullptr;
}

void AClimbingAIController::StartClimbing()
{
    UBlackboardComponent* OwnerBlackboardComp = GetBlackboardComponent();
    if (OwnerBlackboardComp == nullptr) return;

    OwnerBlackboardComp->SetValueAsVector(FName(TEXT("MainTargetLocation")), FVector::Zero());
}

void AClimbingAIController::StopClimbing()
{
    UBlackboardComponent* OwnerBlackboardComp = GetBlackboardComponent();
    if (OwnerBlackboardComp == nullptr) return;

    OwnerBlackboardComp->ClearValue(FName(TEXT("MainTargetLocation")));
}
