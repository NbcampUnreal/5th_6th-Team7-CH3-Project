// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangedAIController.h"
#include "NavigationSystem.h"

#include "Enemy/BaseEnemy.h"

FVector ARangedAIController::GetAttackLocation(FVector TargetLocation)
{
    ABaseEnemy* MyCharacter = Cast<ABaseEnemy>(GetCharacter());
    if (MyCharacter == nullptr)
        return FVector::ZeroVector;

    UWorld* World = GetWorld();
    if (World == nullptr)
        return FVector::ZeroVector;

    const FVector SelfLocation = MyCharacter->GetActorLocation();

    FHitResult Hit;
    FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(AI_RangedLineOfSight), true, MyCharacter);
    TraceParams.bReturnPhysicalMaterial = false;
    TraceParams.AddIgnoredActor(MyCharacter);

    const bool bHit = World->LineTraceSingleByChannel(
        Hit,
        SelfLocation + FVector(0.f, 0.f, 50.f),
        TargetLocation + FVector(0.f, 0.f, 50.f),
        ECollisionChannel::ECC_GameTraceChannel1,
        TraceParams
    );

    if (!bHit)
    {
        return SelfLocation;
    }

    UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);
    if (!NavSystem)
        return TargetLocation;

    FNavLocation ProjectedLocation;
    if (NavSystem->ProjectPointToNavigation(TargetLocation, ProjectedLocation, FVector(100, 100, 100.0f)))
    {
        return ProjectedLocation.Location;
    }

    return TargetLocation;
}