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


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZWAVE_API UCharacterActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCharacterActionComponent();

public:
	void InitRefs(UCharacterMovementComponent* InMoveComp, USpringArmComponent* InSpringArm, UCameraComponent* InCamera);

	void Move(const FInputActionValue& Value);
	void StopMove();
	void Look(const FInputActionValue& Value);
	void StartJump();
	void StopJump();
	void StartSprint();
	void StopSprint();
	void StartShoulder();
	void StopShoulder();
	void Shooting(ATaskPlayer* OwnerChar, EShootType ShootType);
	void StopShooting();
	void TickAction(float DeltaTime);
	void Reload(ATaskPlayer* OwnerChar, EShootType ShootType);
	void EquipChange(ATaskPlayer* OwnerChar, EShootType ShootType);
	void DryShot(ATaskPlayer* OwnerChar, EShootType ShootType);
	void Attacked(AActor* DamageCauser);
	void Die();
	void Grenade(ATaskPlayer* OwnerChar);
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
	TObjectPtr<UAnimMontage> GrenadeMontage = nullptr;

	UPROPERTY()        
	UAnimInstance* CachedAnimInstance = nullptr;

	bool bNotifyBound = false;

	void EnsureBindMontageNotifies(UAnimInstance* Anim);
	void UnbindMontageNotifies(ATaskPlayer* Player);

	UFUNCTION(BlueprintCallable)
	void OnMontageNotifyBegin(FName NotifyName);
private:
	TWeakObjectPtr<UCharacterMovementComponent> MoveComp;
	TWeakObjectPtr<USpringArmComponent>        SpringArm;
	TWeakObjectPtr<UCameraComponent>           Camera;

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
	bool bDryShooting = false;
	void ClearDryShotBlock();

	void UpdateShoulder(float DeltaTime);
	void CheckMoveSpeed(float DeltaTime);
	void SetMeshDir(float DeltaTime);
	float ConvertAngle(float InDeg);

};
