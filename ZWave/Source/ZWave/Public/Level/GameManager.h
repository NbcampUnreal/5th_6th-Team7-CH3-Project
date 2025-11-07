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
class UEffectApplyManager;
class UEffectBase;
class ATurret;
class UEnemySpawnManager;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlackoutStateChangedSignature, bool, bIsBlackoutActive);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStimStateChangedSignature, bool, bIsNowOnCooldown);

UCLASS()
class ZWAVE_API UGameManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

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

	UFUNCTION(BlueprintPure, Category = "GameManger")
	EGameState GetCurrentState() const { return CurrentState; }

	UFUNCTION(BlueprintCallable, Category = "Turret")
	bool SpawnTurret(int32 SpawnPointIndex);

	UFUNCTION(BlueprintCallable, Category = "DoorControl")
	void UpgradeDoorBattery(float time);

	void GetRewardBioCoin(int32 value);
	void BindPlayerEvents(APawn* PlayerPawn);
	void BeginPreparationPhase();
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
	float DefaultPreparationTime = 30.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Blackout")
	float BlackoutChancePerWave = 0.3f;

	UPROPERTY(EditDefaultsOnly, Category = "Blackout")
	float BlackoutDuration = 20.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blackout")
	bool bIsBlackoutActive = false;

	bool bBlackoutUsedThisWave = false;

	UPROPERTY(Transient)
	TObjectPtr<UEffectApplyManager> EffectManager;

	UPROPERTY(VisibleAnywhere, Category = "Stim System")
	TSubclassOf<UEffectBase> StimEffectClass;

	UPROPERTY()
	TArray<FTransform> TurretSpawnTransforms;

	UPROPERTY(VisibleAnywhere, Category = "Turret")
	TSubclassOf<ATurret> TurretClassToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Stats")
	int32 CumulativeKills = 0;

	UPROPERTY()
	TWeakObjectPtr<UEnemySpawnManager> EnemySpawnManager;

	EGameState CurrentState = EGameState::EGS_None;
	int32 CurrentWaveNumber = 0;
	FTimerHandle PrepPhaseTimerHandle;
	FTimerHandle GameStartTimerHandle;
	FTimerHandle BlackoutTimerHandle;

	UFUNCTION()
	void HandleEnemyKilled(ABaseCharacter* DiedEnemy);

	UFUNCTION()
	void OnPlayerDied(AActor* DiedActor);

	UFUNCTION(BlueprintPure, Category = "Game Stats")
	int32 GetCumulativeKills() const { return CumulativeKills; }

	void StartGame();
	void BeginCombatPhase();
	void EndGame(bool bPlayerWon);
	void StartBlackoutEvent();
	void EndBlackoutEvent();
	AZWaveGameState* GetGameState();

};
