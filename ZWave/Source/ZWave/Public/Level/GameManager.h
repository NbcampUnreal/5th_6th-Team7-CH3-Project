// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Level/ZWaveTypes.h"
#include "GameManager.generated.h"

class UWaveManager;
class AZWaveGameState;
class APawn;

UCLASS()
class ZWAVE_API UGameManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void BindPlayerEvents(APawn* PlayerPawn);

	void StartGame();

	void BeginPreparationPhase();

	void BeginCombatPhase();

	void EndGame(bool bPlayerWon);

	UFUNCTION(BlueprintPure, Category = "GameManger")
	EGameState GetCurrentState() const { return CurrentState; }

protected:
	UFUNCTION()
	void OnPlayerDied(AActor* DiedActor);

	AZWaveGameState* GetGameState();

	UPROPERTY()
	TWeakObjectPtr<UWaveManager> WaveManager;

	UPROPERTY()
	TWeakObjectPtr<AZWaveGameState> GameState;

	EGameState CurrentState = EGameState::EGS_None;

	int32 CurrentWaveNumber = 0;

	FTimerHandle PrepPhaseTimerHandle;
	FTimerHandle GameStartTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Game Wave")
	float DefaultPreparationTime = 5.0f;
};
