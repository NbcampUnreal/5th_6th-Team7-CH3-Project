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

	CurrentSlot = EEquipSlot::None;

	if (WeaponDefinitionMap.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Setting Not Completed!"));
		return;
	}

	for (const auto& Def : WeaponDefinitionMap)
	{
		SetSlotData(Def.Key, Def.Value);
	}
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
		ACharacter* OwnChara = Cast<ACharacter>(GetOwner());
		if(OwnChara == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Owner Is Null!"));
			return false;
		}

		if (CurrentSlot != EEquipSlot::None)
		{
			SlotMaps[CurrentSlot]->Unequip();
			UnEquip(CurrentSlot);
		}
		CurrentSlot = Slot;

		FindWeapon->SetActorHiddenInGame(false);
		FindWeapon->SetActorEnableCollision(true);
		FindWeapon->SetActorTickEnabled(true);

		SlotMaps[CurrentSlot]->Equip(OwnChara);

		// Attach
		AttachWeaponToOwner(FindWeapon, WeaponDefinitionMap[CurrentSlot]);

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

		if (WeaponActor->Init(WeaponDef) == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponDef is Not Valid to Weapon Class!"));
			WeaponActor->Destroy();
			return;
		}

		if (TObjectPtr<AWeaponBase>* FoundPtr = SlotMaps.Find(Slot))
		{
			if (AWeaponBase* FindWeapon = FoundPtr->Get())
			{
				FindWeapon->Destroy();
				*FoundPtr = nullptr;
			}
		}

		UGameplayStatics::FinishSpawningActor(WeaponActor, FTransform::Identity);
		SlotMaps.Add(Slot, WeaponActor);
		UnEquip(Slot);
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

