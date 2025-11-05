// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/RangedAIController.h"
#include "NavigationSystem.h"

#include "Enemy/RangedEnemy.h"
#include "Weapon/EquipComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

bool ARangedAIController::CheckCondition(AActor* Target)
{
    ARangedEnemy* MyCharacter = Cast<ARangedEnemy>(GetCharacter());
    if (!MyCharacter) return false;

    UEquipComponent* EquipComponent = MyCharacter->FindComponentByClass<UEquipComponent>();
    if (!EquipComponent) return false;

    AWeaponBase* Weapon = EquipComponent->GetCurrentWeapon();
    if (!Weapon) return false;

    UBlackboardComponent* OwnerBlackboardComp = GetBlackboardComponent();
    if (OwnerBlackboardComp == nullptr) return false;

    OwnerBlackboardComp->SetValueAsVector(FName(TEXT("SecondaryTargetLocation")), Target->GetActorLocation());

    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(AI_RangedSight), true, MyCharacter);
    Params.bReturnPhysicalMaterial = false;
    Params.AddIgnoredActor(MyCharacter);

    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        Hit,
        MyCharacter->GetActorLocation() + FVector(0, 0, 50),
        Target->GetActorLocation() + FVector(0, 0, 50),
        ECollisionChannel::ECC_GameTraceChannel1,
        Params
    );
    bool bCanAttack = (bHit && Hit.GetActor() == Target);

    //FColor Color = bCanAttack ? FColor::Green : FColor::Red;
    //DrawDebugLine(GetWorld(), MyCharacter->GetActorLocation() + FVector(0, 0, 50), Target->GetActorLocation() + FVector(0, 0, 50), Color, false, 0.2f, 0, 1.5f);

    if (!bCanAttack)
    {
        OwnerBlackboardComp->ClearValue(FName(TEXT("AttackLocation")));
        return false;
    }
    TargetVector = Target->GetActorLocation();
    OwnerBlackboardComp->SetValueAsVector(FName(TEXT("MainTargetLocation")), Target->GetActorLocation());
    OwnerBlackboardComp->SetValueAsVector(FName(TEXT("AttackLocation")), MyCharacter->GetActorLocation());
    return true;
}

void ARangedAIController::GetTargetVector(FVector& OutTargetVector) const
{
    OutTargetVector = TargetVector;
}
