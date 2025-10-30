// Fill out your copyright notice in the Description page of Project Settings.


#include "Base/BaseCharacter.h"
#include "Weapon/EquipComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Health = 1;
	MaxHealth = Health;
	SpeedMultiply = 1.f;
	Armor = 0.f;

	EquipComponent = CreateDefaultSubobject<UEquipComponent>(TEXT("EquipComponent"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	MaxHealth = Health;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseCharacter::Attacked(AActor* DamageCauser, float Damage)
{
	ApplyDamage(Damage);
}

void ABaseCharacter::ApplyDamage(float Damage, bool CheckArmor)
{
	//방어력 1일경우 데미지 적용 비율 93프로 ~ 방어력 10일 경우 약 63프로 ~ 100일경우 27프로의 데미지가 적용됨
	//방어력이 음수일경우 0으로 취급
	float ReductionRatio = CheckArmor ? (1.f / FMath::Log2((FMath::Max(Armor, 0.f) / 10) + 2)) : 1.f;
	float ApplyDamage = Damage * ReductionRatio;

	Health = FMath::Max(0.f, Health - Damage);

	if (Health <= 0.f)
	{
		Die();
	}
}

void ABaseCharacter::Die()
{
	SetLifeSpan(0.05f);
}

float ABaseCharacter::GetMaxHealth()
{
	return MaxHealth;
}

void ABaseCharacter::SetHealth(float SetHealAmount)
{
	Health = FMath::Clamp(Health + SetHealAmount, 0.0f, MaxHealth);
}

