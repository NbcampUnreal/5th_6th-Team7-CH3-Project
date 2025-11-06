// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "CharacterActionComponent.generated.h"

class UCharacterMovementComponent;
class USpringArmComponent;
class UCameraComponent; 
class ATaskPlayer;
enum class EShootType : uint8;

UENUM(BlueprintType)
enum class ECharacterMoveDirection : uint8
{
	Foward,
	Back,
	Left,
	Right
};

UENUM(BlueprintType)
enum class EHitDirection : uint8
{
	Front,
	Back,
	Left,
	Right
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZWAVE_API UCharacterActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterActionComponent();

public:
	void InitRefs(ATaskPlayer* InPlayerCharacter ,UCharacterMovementComponent* InMoveComp, USpringArmComponent* InSpringArm, UCameraComponent* InCamera);

	void Move(const FInputActionValue& Value);
	void StopMove();
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void StartSprint();
	void StopSprint();
	void StartShoulder();
	void StopShoulder();
	void Shooting(EShootType ShootType, float ShootSpeedMultiply);
	void StopShooting();
	void TickAction(float DeltaTime);
	void Reload(EShootType ShootType, float ReloadSpeedMultiply);
	void EquipChange(EShootType ShootType);
	void DryShot( EShootType ShootType);
	void Grenade();
	void ActiveFuildObject();
	
	void Attacked(AActor* DamageCauser);
	void Die();
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsSprinting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bShoulder = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsShooting = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Facing")
	float BP_MoveDirDeltaYawDeg = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Facing")
	ECharacterMoveDirection MoveDirection = ECharacterMoveDirection::Foward;

	UFUNCTION()
	void ChangeSpeedMultiply(float Multiply = 1.f);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> RifleFireMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> RifleReloadMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> RifleEquipMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> RifleDryShotMontage = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> PistolFireMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> PistolReloadMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> PistolEquipMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> PistolDryShotMontage = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> ShotgunFireMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> ShotgunReloadMontage = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> GrenadeMontage = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> FrontHitMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> BackHitMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> LeftHitMontage = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> RightHitMontage = nullptr;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> DeathMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Anim")
	TObjectPtr<UAnimMontage> ActiveMontage = nullptr;


	UPROPERTY()        
	UAnimInstance* CachedAnimInstance = nullptr;

	bool bNotifyBound = false;

	void EnsureBindMontageNotifies(UAnimInstance* Anim);
	void UnbindMontageNotifies();

	UFUNCTION(BlueprintCallable)
	void OnMontageNotifyBegin(FName NotifyName);
private:
	TWeakObjectPtr<UCharacterMovementComponent> MoveComp;
	TWeakObjectPtr<USpringArmComponent>			SpringArm;
	TWeakObjectPtr<UCameraComponent>			Camera;
	TWeakObjectPtr <ATaskPlayer>				OwnerCharacter;

	float MaxSprintSpeed = 600.f;
	float NormalWalkSpeed = 300.f;
	float ShoulderSpeed = 150.f;
	float SpeedMultiply = 1.f;

	float NormalArmLength = 200.f;
	float ShoulderArmLength = 65.f;
	float NormalSocketOffsetY = 0.f;
	float ArmLerpSpeed = 800.f;

	float SpringArmNormalSocketOffsetY = 65.f;
	float SpringArmShoulderSocketOffsetY = 65.f;
	float SpringArmSocketOffsetZ = 85.f;

	float SprintAnimationSpeed = 1000.f;
	float SprintAnimationRecovorySpeed = 1000.f;

	float MoveSpeed = 0.0f;

	float MeshTurnSpeed = 10.f;
	float FaceDirSpeedThreshold = 5.f;
	float BaseMeshYawOffsetDeg = -90.f;
	float LastDiagonalYawDeg = 0.f;
	float MeshYawOffsetDeg = 0.f;
	float DiagonalDeadZone = 0.1f;
	float MoveDirDeltaYawDeg = 0.f;
	FVector2D LastMoveInput = FVector2D::ZeroVector;

	FTimerHandle DryShotBlockHandle;
	FTimerHandle StopMotionHandler;
	FTimerHandle DeathHandler;
	bool bIsDead = false;
	bool bDryShooting = false;
	void ClearDryShotBlock();
	void SetStopMotion();
	void GameOver();

	void UpdateShoulder(float DeltaTime);
	void CheckMoveSpeed(float DeltaTime);
	void SetMeshDir(float DeltaTime);
	float ConvertAngle(float InDeg);
	UAnimMontage* GetAttackedMontage(EHitDirection Direction);

	float GetSpeedValue(float Speed);

	AActor* ActivateObject;
};
