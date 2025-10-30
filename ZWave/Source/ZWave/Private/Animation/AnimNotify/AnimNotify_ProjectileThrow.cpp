// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_ProjectileThrow.h"
#include "Weapon/EquipComponent.h"
#include "Weapon/ProjectileWeapon.h"

void UAnimNotify_ProjectileThrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp == nullptr)
		return;

	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
		return;

	UEquipComponent* EquipComponent = Owner->FindComponentByClass<UEquipComponent>();
	if (EquipComponent == nullptr)
		return;

	if (AWeaponBase* WeaponActor = EquipComponent->GetConsumeItem())
	{
		WeaponActor->Attack();
	}
}
