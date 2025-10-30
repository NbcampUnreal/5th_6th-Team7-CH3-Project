// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/MeleeWeapon.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "DamageCalculator/DamageCalculator.h"

AMeleeWeapon::AMeleeWeapon()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SphereCollision->SetGenerateOverlapEvents(false);
	SetRootComponent(SphereCollision);
}

void AMeleeWeapon::Attack()
{
	if (GetWorld() == nullptr)
		return;

	if (OverlappedEnemies.Num() == 0)
		return;

	if (CanAttack() == false ||
		bEquipped == false ||
		OwningCharacter == nullptr)
		return;

	for (TObjectPtr<AActor> TargetActor : OverlappedEnemies)
	{
		if (IsDamagableActor(TargetActor) == true)
		{
			UDamageCalculator::DamageCalculate(
				GetWorld(),
				OwningCharacter,
				TargetActor,
				MeleeWeaponStat.AttackPower,
				BaseEffectClasses);
		}
	}
}

bool AMeleeWeapon::Init(const UWeaponDefinition* WeaponDefinition)
{
	const UMeleeWeaponDefinition* MeleeDefinition = Cast<UMeleeWeaponDefinition>(WeaponDefinition);
	if (MeleeDefinition == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's Not MeleeWeaponDef!"));
		return false;
	}

	MeleeWeaponStat = MeleeDefinition->MeleeWeaponStat;
	SphereCollision->InitSphereRadius(MeleeWeaponStat.Radius);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnSphereBeginOverlap);

	return true;
}

void AMeleeWeapon::Equip(ACharacter* NewOwner)
{
	Super::Equip(NewOwner);
}

void AMeleeWeapon::Unequip()
{
	Super::Unequip();
}

void AMeleeWeapon::StartAttack()
{
	SphereCollision->SetGenerateOverlapEvents(true);
}

void AMeleeWeapon::EndAttack()
{
	SphereCollision->SetGenerateOverlapEvents(false);
	Attack();
	OverlappedEnemies.Empty();
	bCanAttack = false;

	GetWorldTimerManager().SetTimer(AttackTimer, [this]()
		{bCanAttack = true; }
	, MeleeWeaponStat.AttackRate, false);
}

void AMeleeWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor &&
		OtherActor != GetOwner() &&
		OtherActor != OwningCharacter)
	{
		OverlappedEnemies.AddUnique(OtherActor);
	}
}
