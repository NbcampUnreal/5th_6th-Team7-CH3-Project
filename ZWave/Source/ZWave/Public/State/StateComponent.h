// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StateComponent.generated.h"

UENUM(BlueprintType)
enum class EStateType : uint8
{
	EST_None,
	EST_Stagger, // 경직 효과 중첩 방지를 위해 추가
	EST_Stun,
	EST_Decoy
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZWAVE_API UStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStateComponent();

	void SetState(EStateType SetType);
	FORCEINLINE EStateType GetCurrentState() const { return CurrentState; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	EStateType CurrentState;
		
};
