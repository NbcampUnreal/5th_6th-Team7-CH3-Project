// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Base/BaseCharacter.h"
#include "InputActionValue.h"
#include "TaskPlayer.generated.h"


UENUM(BlueprintType)
enum class EPlayerShopStat : uint8
{
	MaxHealth,
	MoveSpeedMultiply,
	ReloadSpeedMultiply,
	ShotSpeedMultiplay,
};

class UCharacterActionComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class AShootWeapon;
class UIngameHUD;
enum class EShootType : uint8;

UCLASS()
class ZWAVE_API ATaskPlayer : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ATaskPlayer();

	UFUNCTION(BlueprintCallable)
	const bool IsDead() { return Health <= 0.f; };

	void AttachWeaponTo(const FName SocketName);

	FORCEINLINE float GetSpeedMultiply() const { return SpeedMultiply; }

	UFUNCTION(BlueprintCallable)
	void AddPlayerStat(EPlayerShopStat statType, float value);


	UFUNCTION()
	void AddActiveObject(AActor* inActiveObject);

	UFUNCTION()
	void RemoveActiveObject(AActor* inActiveObject);

	UFUNCTION()
	void PruneInvalid();

	UFUNCTION()
	AActor* PickNearestActiveObject();
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attacked(AActor* DamageCauser, float Damage) override;
	virtual void SetHealth(float SetHealAmount) override;
	virtual void Die() override;

	void EquipFirstSlot();
	void EquipSecondSlot();
	void EquipThirdSlot();
	void EquipChange();
	void CheckShooting();
	void Reload();
	void Grenade();
	void ActiveFieldObject();

	UFUNCTION()
	void ShootingAction();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	EShootType GetShootType() const;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskPlayer|Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskPlayer|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskPlayer|Action", meta = (AllowPrivateAccess = "true"))
	UCharacterActionComponent* ActionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskPlayer|Inventory", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskPlayer|AimSet", meta = (AllowPrivateAccess = "true"))
	float ControllerPitch = 0.0f;

	UPROPERTY()
	AShootWeapon* NowShootWeapon;

	UIngameHUD* GetIngameHud();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TaskPlayer|ShopStat", meta = (AllowPrivateAccess = "true"))
	float MoveSpeedMultiply;

	float ReloadSpeedMultiply;
	float ShotSpeedMultiplay;

	UPROPERTY()
	TArray<TWeakObjectPtr<AActor>> InteractCandidates;
};
