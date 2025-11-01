// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/Turret.h"

#include "Components/SphereComponent.h"
#include "TimerManager.h"

#include "Weapon/EquipComponent.h"
#include "Weapon/WeaponBase.h"
#include "Enemy/BaseEnemy.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(MeshComp);

	SphereComp->InitSphereRadius(AwarenessRange);

	EquipComp = CreateDefaultSubobject<UEquipComponent>(TEXT("EquipComp"));
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATurret::OnSphereBeginOverlap);

	this->FireInterval = 60.0f / WeaponRPM;
	//Weapon = EquipComp->GetCurrentWeapon();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurret::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ABaseEnemy::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT("Target is overlaped: %s"), *OtherActor->GetActorNameOrLabel());
		if (Target == nullptr)
		{
			Target = static_cast<ABaseEnemy*>(OtherActor);

			//Weapon->Attack();
			Attack();
			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ATurret::Attack, FireInterval, true);
		}
	}
}

void ATurret::Attacked(AActor* DamageCauser, float Damage)
{
	ApplyDamage(Damage, false);
}

void ATurret::ApplyDamage(float Damage, bool CheckArmor)
{
	Health -= Damage;

	if (Health <= 0)
	{
		Health = 0;
		Die();
	}
}

void ATurret::Die()
{
	SetLifeSpan(0.5f);
}

void ATurret::Attack()
{
	if (Target == nullptr) {
		UE_LOG(LogTemp, Display, TEXT("Null Target"));
		return;
	}

	UE_LOG(LogTemp, Display, TEXT("Attack"));

	//Weapon->Attack();
	Target->Attacked(this, 20);
}
