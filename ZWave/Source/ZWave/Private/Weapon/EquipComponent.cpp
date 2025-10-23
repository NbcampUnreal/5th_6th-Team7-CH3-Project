// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/EquipComponent.h"
#include "Weapon/WeaponBase.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>
#include "Components/SkeletalMeshComponent.h"

UEquipComponent::UEquipComponent()
{
}

void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponDefinitionMap.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Setting Not Completed!"));
		return;
	}

	SetSlotData(EEquipSlot::First, WeaponDefinitionMap[EEquipSlot::First]);
	Equip(EEquipSlot::First);
}

bool UEquipComponent::Equip(EEquipSlot Slot)
{
	if (CurrentSlot == Slot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Slot Already Equipped!"));
		return false;
	}

	if (AWeaponBase* FindWeapon = SlotMaps.Find(Slot)->Get())
	{
		UnEquip(Slot);
		CurrentSlot = Slot;

		FindWeapon->SetActorHiddenInGame(false);
		FindWeapon->SetActorEnableCollision(true);
		FindWeapon->SetActorTickEnabled(true);

		// Attach
		AttachWeaponToOwner(FindWeapon, WeaponDefinitionMap[Slot]);

		return true;
	}

	UE_LOG(LogTemp, Warning, TEXT("Target SlotData Is Empty!"));
	return false;
}

void UEquipComponent::UnEquip(EEquipSlot Slot)
{
	if (AWeaponBase* FindWeapon = SlotMaps.Find(Slot)->Get())
	{
		FindWeapon->SetActorHiddenInGame(true);
		FindWeapon->SetActorEnableCollision(false);
		FindWeapon->SetActorTickEnabled(false);
	}
}

void UEquipComponent::SetSlotData(EEquipSlot Slot, const UWeaponDefinition* WeaponDef)
{
	if (WeaponDef == nullptr ||
		GetOwner() == nullptr)
		return;

	if (UWorld* World = GetWorld())
	{
		AWeaponBase* WeaponActor = World->SpawnActorDeferred<AWeaponBase>(
			WeaponDef->WeaponClass,
			FTransform::Identity,
			GetOwner(),
			Cast<APawn>(GetOwner())
		);

		if (WeaponActor == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon SpawnDeffered Failed!"));
			return;
		}

		if (AWeaponBase* FindWeapon = SlotMaps.Find(Slot)->Get())
		{
			FindWeapon->Destroy();
			SlotMaps[Slot] = nullptr;
		}

		WeaponActor->Init(WeaponDef);
		UGameplayStatics::FinishSpawningActor(WeaponActor, FTransform::Identity);
		SlotMaps[Slot] = WeaponActor;
	}
}

void UEquipComponent::ClearSlotData(EEquipSlot Slot)
{
	// 현재 장착된 슬롯을 삭제하는 것에 대한 처리는 X
	// (차후 기획 관련으로 변동 가능)
	if (CurrentSlot == Slot)
		return;

	if (AWeaponBase* FindWeapon = SlotMaps.Find(Slot)->Get())
	{
		FindWeapon->Destroy();
		SlotMaps[Slot] = nullptr;
	}
}

void UEquipComponent::AttachWeaponToOwner(AWeaponBase* Weapon, const UWeaponDefinition* WeaponDef)
{
	if (Weapon == nullptr ||
		WeaponDef == nullptr)
		return;

	ACharacter* OwnChara = Cast<ACharacter>(GetOwner());
	if (OwnChara == nullptr)
		return;

	USkeletalMeshComponent* SkeletalMesh = OwnChara->GetMesh();
	if (SkeletalMesh == nullptr)
		return;

	const FName SocketName = WeaponDef->WeaponAttachSocket;
	if (Weapon->AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Attached Failed! SocketName : %s"), *SocketName.ToString());
	}
}

