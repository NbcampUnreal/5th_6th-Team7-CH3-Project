// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyStateComponent.generated.h"

UENUM(BlueprintType)
enum class EEnemyStateType : uint8
{
	EST_None,
	EST_Stagger, // 경직 효과 중첩 방지를 위해 추가
	EST_Stun,
	EST_Decoy,
	EST_Death,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZWAVE_API UEnemyStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemyStateComponent();

protected:
	virtual void BeginPlay() override;
public:
	void SetState(EEnemyStateType SetType, float Duration = 0.f);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EEnemyStateType GetCurrentState() const { return CurrentState; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimationMontage")
	TObjectPtr<class UAnimMontage> Montage_Stun;
	float MontageStunPlayRate = 1.5f;
	float MontageStunLength = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimationMontage")
	TObjectPtr<class UAnimMontage> Montage_RecoverStun;
	float MontageRecoverStunPlayRate = 2.0f;
	float MontageRecoverStunLength = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimationMontage")
	TObjectPtr<class UAnimMontage> Montage_Die;

private:
	UPROPERTY(VisibleAnywhere, Category = "State")
	EEnemyStateType CurrentState = EEnemyStateType::EST_None;
	EEnemyStateType PreState = EEnemyStateType::EST_None;

	FTimerHandle StateManageHandle;

	void OnNone();
	void OnStun(const float Duration);
	void RecoverStun();
	void OnDeath();
	
	void ExcuteDestroy();
};
