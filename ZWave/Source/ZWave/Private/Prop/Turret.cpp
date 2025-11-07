// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/Turret.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"

#include "Weapon/EquipComponent.h"
#include "Weapon/WeaponBase.h"
#include "Enemy/BaseEnemy.h"
#include "Effect/EffectApplyManager.h"
#include "DamageCalculator/DamageCalculator.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollider"));
	RootComponent = CapsuleComp;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetupAttachment(RootComponent);

	SphereComp->SetSphereRadius(AwarenessRange*2);

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

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateToTarget(DeltaTime);
}

void ATurret::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ABaseEnemy::StaticClass()))
	{
		if (Target == nullptr)
		{
			SetTarget(static_cast<ABaseEnemy*>(OtherActor));
			
			//Attack();
			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ATurret::Attack, FireInterval, true);
		}
	}
}

FGenericTeamId ATurret::GetGenericTeamId() const
{
	return this->TeamID;
}

float ATurret::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (const FZWaveDamageEvent* CustomDamageEvent = static_cast<const FZWaveDamageEvent*>(&DamageEvent))
	{
		if (UEffectApplyManager* EffectManager = GetWorld()->GetSubsystem<UEffectApplyManager>())
		{
			EffectManager->ApplyEffect(this, CustomDamageEvent->EffectArray, CustomDamageEvent->Duration);
		}


		Attacked(DamageCauser, DamageAmount); //데미지 계산 후 넘겨줄 수도 있고 아니면 그냥 이렇게 쓸 수도 있을 듯
	}

	return DamageAmount;
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

	/*SetLifeSpan(0.5f);*/	
	if (Mesh != nullptr) {
		UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

		if (AnimInstance != nullptr) {
			AnimInstance->Montage_Play(Montage_Destory);
		}
	}
}


void ATurret::SetTarget(ABaseEnemy* NewTarget)
{
	if (NewTarget == nullptr)
	{
		Target = nullptr;
	} 
	else
	{
		if (Target == nullptr)
		{
			Target = NewTarget;
			UE_LOG(LogTemp, Display, TEXT("New Target: %s"), *NewTarget->GetActorNameOrLabel());
		}
	}
}

void ATurret::Attack()
{
	if (Target == nullptr || Health <= 0.f) {
		return;
	}

	FVector TargetDirection = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator TargetRot = FRotator(0, TargetDirection.Rotation().Yaw, 0);

	FRotator CurrentRot = GetActorRotation();

	if (FMath::Abs(TargetRot.Yaw - CurrentRot.Yaw) > 10) {

		return;
	}
	//Weapon->Attack();
	if (Mesh != nullptr) {
		UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
		
		if (AnimInstance != nullptr) {
			AnimInstance->Montage_Play(AttackMontage);
		}
	}

	Target->Attacked(this, this->WeaponDamage);
	if (Target->GetHealth() <= 0.f)
	{
		StopAttack();
	}
}

void ATurret::RotateToTarget(float DeltaTime)
{
	if (Target == nullptr) return;

	FVector TargetDirection = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator TargetRot = FRotator(0, TargetDirection.Rotation().Yaw, 0);

	FRotator CurrentRot = GetActorRotation();
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaTime, RotationSpeed);
	SetActorRotation(NewRot);
}

void ATurret::StopAttack()
{
	SetTarget(nullptr);

	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
	SearchEnemy();
}

void ATurret::SearchEnemy()
{
	TArray<FHitResult> HitResults;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		GetActorLocation(),
		GetActorLocation(),
		FQuat::Identity,
		ECC_Visibility,                  // 사용할 충돌 채널 설정
		FCollisionShape::MakeSphere(this->AwarenessRange)
	);

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (Hit.GetActor()->IsA(ABaseEnemy::StaticClass()))
			{
				ABaseEnemy* Temp = static_cast<ABaseEnemy*>(Hit.GetActor());

				if (Temp->GetHealth() > 0)
				{
					SetTarget(Temp);
					GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ATurret::Attack, FireInterval, true);
					break;
				}
			}
		}
	}
}
