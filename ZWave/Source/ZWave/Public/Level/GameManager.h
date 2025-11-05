// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Level/ZWaveTypes.h"
#include "GameManager.generated.h"

class UWaveManager;
class AZWaveGameState;
class APawn;
class ADoorControlPanel;
class UPointLightComponent;
class USpotLightComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlackoutStateChangedSignature, bool, bIsBlackoutActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStimStateChangedSignature, bool, bIsNowOnCooldown);

UCLASS()
class ZWAVE_API UGameManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void BindPlayerEvents(APawn* PlayerPawn);

	UFUNCTION(BlueprintPure, Category = "GameManger")
	EGameState GetCurrentState() const { return CurrentState; }

	void BeginPreparationPhase();

	UPROPERTY(BlueprintAssignable, Category = "Blackout")
	FOnBlackoutStateChangedSignature OnBlackoutStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Stim System")
	FOnStimStateChangedSignature OnStimStateChanged;

	UPROPERTY(VisibleAnywhere, Category = "Stim System")
	bool bIsStimUsedThisWave = false;

	UPROPERTY(EditDefaultsOnly, Category = "Stim System")
	float StimBuffDuration = 10.0f;

	UFUNCTION(BlueprintCallable, Category = "Stim System")
	void UseStim(AActor* Interactor);

	UFUNCTION(BlueprintPure, Category = "Stim System")
	bool IsStimOnCooldown() const { return bIsStimUsedThisWave; }

	bool GetIsBlackOutActive() { return bIsBlackoutActive; }

protected:
	UPROPERTY()
	TWeakObjectPtr<UWaveManager> WaveManager;

	UPROPERTY()
	TWeakObjectPtr<AZWaveGameState> GameState;

	UPROPERTY()
	TArray<TObjectPtr<UPointLightComponent>> AllManagedPointLights;

	UPROPERTY()
	TArray<TObjectPtr<USpotLightComponent>> AllManagedSpotLights;

	UPROPERTY()
	TArray<TObjectPtr<ADoorControlPanel>> AllControlPanelsInLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	float DefaultPreparationTime = 5.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Blackout")
	float BlackoutChancePerWave = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Blackout")
	float BlackoutDuration = 15.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blackout")
	bool bIsBlackoutActive = false;

	bool bBlackoutUsedThisWave = false;


	EGameState CurrentState = EGameState::EGS_None;

	int32 CurrentWaveNumber = 0;

	FTimerHandle PrepPhaseTimerHandle;
	FTimerHandle GameStartTimerHandle;
	FTimerHandle BlackoutTimerHandle;

	UFUNCTION()
	void OnPlayerDied(AActor* DiedActor);

	void StartGame();

	void BeginCombatPhase();

	void EndGame(bool bPlayerWon);

	void StartBlackoutEvent();

	void EndBlackoutEvent();

	AZWaveGameState* GetGameState();




};
