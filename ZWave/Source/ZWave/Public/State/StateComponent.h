<<<<<<< HEAD
// Fill out your copyright notice in the Description page of Project Settings.
=======
﻿// Fill out your copyright notice in the Description page of Project Settings.
>>>>>>> 0209b5b5425e4dc776c57e7e6746021719593a68

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

<<<<<<< HEAD

=======
UENUM(BlueprintType)
enum class EStateType : uint8
{
	EST_None,
	EST_Stagger, // 경직 효과 중첩 방지를 위해 추가
	EST_Stun,
	EST_Decoy
};
>>>>>>> 0209b5b5425e4dc776c57e7e6746021719593a68


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZWAVE_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateComponent();

<<<<<<< HEAD
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
=======
	void SetState(EStateType SetType);
	FORCEINLINE EStateType GetCurrentState() const { return CurrentState; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	EStateType CurrentState;
		
>>>>>>> 0209b5b5425e4dc776c57e7e6746021719593a68
};
