// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZWAVE_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	//void SetState(EStateType SetType, float Duration = 0.f);

	//UFUNCTION(BlueprintCallable)
	//FORCEINLINE EStateType GetCurrentState() const { return CurrentState; }

protected:
	//class ABaseEnemy* OwningCharacter;
	//class ABaseAIController* AIController;
	//class USkeletalMeshComponent* Mesh;
	//class UAnimInstance* AnimInstance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimationMontage")
	TObjectPtr<class UAnimMontage> Montage_Stun;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimationMontage")
	TObjectPtr<class UAnimMontage> Montage_RecoverStun;


private:
	//UPROPERTY(VisibleAnywhere, Category = "State")
	//EStateType CurrentState = EStateType::EST_None;;

	//FTimerHandle StateManageHandle;

	//void OnNone();
	//void OnStun(const float Duration);
	//void OnRecoverStun();
};
