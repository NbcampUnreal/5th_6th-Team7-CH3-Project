// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_MeleeAttackEnd.h"
#include "Weapon/EquipComponent.h"
#include "Weapon/MeleeWeapon.h"

void UAnimNotify_MeleeAttackEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp == nullptr)
		return;

	AActor* Owner = MeshComp->GetOwner();
	if (Owner == nullptr)
		return;

	UEquipComponent* EquipComponent = Owner->FindComponentByClass<UEquipComponent>();
	if (EquipComponent == nullptr)
		return;

	if (AWeaponBase* WeaponActor = EquipComponent->GetCurrentWeapon())
	{
		if (AMeleeWeapon* MeleeWeapon = Cast<AMeleeWeapon>(WeaponActor))
		{
			MeleeWeapon->EndAttack();
		}
	}
}
