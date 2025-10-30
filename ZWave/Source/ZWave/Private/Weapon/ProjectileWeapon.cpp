// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileWeapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "GameFramework/Character.h"

AProjectileWeapon::AProjectileWeapon()
{
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));

	SphereCollision->InitSphereRadius(16.f);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionObjectType(ECC_WorldDynamic);

	// 모두 Block: 부딪히면 ProjectileMovement가 Stop/Bounce 이벤트를 발생
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Block);

	SphereCollision->SetNotifyRigidBodyCollision(false);
	SphereCollision->SetGenerateOverlapEvents(false);
	SetRootComponent(SphereCollision);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent); 
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetSimulatePhysics(false);

	TrailComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Trail"));
	TrailComponent->SetupAttachment(StaticMeshComponent);
	TrailComponent->bAutoActivate = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SphereCollision;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->bShouldBounce = false;

	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AProjectileWeapon::OnProjectileStop);
}

void AProjectileWeapon::Attack()
{
    // 인벤토리 조건 검사 예정
	// 다만 그 경우는 이 클래스 상속하고 아이템 Idx 를 받아 Inventory 검사 필요
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	GetWorldTimerManager().ClearTimer(FuseTimer);

    ThrowFromOwner();
}

bool AProjectileWeapon::Init(const UWeaponDefinition* WeaponDefinition)
{
	const UProjectileWeaponDefinition* ProjDefinition = Cast<UProjectileWeaponDefinition>(WeaponDefinition);
	if (ProjDefinition == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's Not ProjectileWeaponDef!"));
		return false;
	}

	ProjectileWeaponStat = ProjDefinition->ProjectileWeaponStat;
	ProjectileMovement->InitialSpeed = ProjectileWeaponStat.InitialSpeed;
	ProjectileMovement->SetActive(false);
	AttachSocketName = ProjDefinition->WeaponAttachSocket;
	AttachToOwner();

    return true;
}

void AProjectileWeapon::ThrowFromOwner()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	FVector Dir = ComputeThrowDirection();

    if (UpDegree > 0.f)
    {
        const FRotator PitchUp(UpDegree, 0.f, 0.f);
        Dir = PitchUp.RotateVector(Dir).GetSafeNormal();
    }

    SetActorRotation(Dir.Rotation());

	SphereCollision->IgnoreActorWhenMoving(GetOwner(), true);

    if (ensure(ProjectileMovement))
    {
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->SetUpdatedComponent(SphereCollision);
		ProjectileMovement->Velocity = Dir * ProjectileWeaponStat.InitialSpeed;
		ProjectileMovement->Activate(true);
    }

    if (ensure(TrailComponent))
    {
        TrailComponent->Activate(true);
    }
}

FVector AProjectileWeapon::ComputeThrowDirection() const
{
    const APawn* PawnOwner = Cast<APawn>(GetOwner());
    if (!PawnOwner)
    {
        return GetActorForwardVector();
    }

    const AController* Controller = PawnOwner->GetController();
    if (const APlayerController* PC = Cast<APlayerController>(Controller))
    {
        FVector CamLoc; FRotator CamRot;
        PC->GetPlayerViewPoint(CamLoc, CamRot);
        return CamRot.Vector();
    }

    const FVector Fwd = PawnOwner->GetActorForwardVector();
    const FVector Dir = (Fwd + FVector::UpVector * 0.2f).GetSafeNormal();
    return Dir;
}

void AProjectileWeapon::AttachToOwner()
{
	ACharacter* OwnChara = Cast<ACharacter>(GetOwner());
	if (OwnChara == nullptr)
		return;

	USkeletalMeshComponent* SkeletalMesh = OwnChara->GetMesh();
	if (SkeletalMesh == nullptr)
		return;

	if (AttachToComponent(SkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachSocketName) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Attached Failed! SocketName : %s"), *AttachSocketName.ToString());
	}

	if (ProjectileMovement)
	{
		ProjectileMovement->StopMovementImmediately();
		ProjectileMovement->Deactivate();
	}

	if (TrailComponent)
	{
		TrailComponent->Deactivate();
	}

	if (SphereCollision)
	{
		SphereCollision->IgnoreActorWhenMoving(GetOwner(), false);
	}

	// 안보이게 설정
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void AProjectileWeapon::OnProjectileStop(const FHitResult& ImpactResult)
{
	if (ProjectileWeaponStat.FuseTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(
			FuseTimer, this, &AProjectileWeapon::Explode, ProjectileWeaponStat.FuseTime, false);
	}
	else
	{
		Explode();
	}
}


void AProjectileWeapon::Explode()
{
	// 해당 AttackPower 부분은 DamageCalculator와 연계 예정

	UGameplayStatics::ApplyRadialDamage(this,
		ProjectileWeaponStat.AttackPower, /*BaseDamage=*/
		GetActorLocation(),
		ProjectileWeaponStat.Radius,   /*Radius=*/
		nullptr,  // DamageTypeClass
		TArray<AActor*>(),
		this,
		GetInstigatorController(),
		true);

	bCanAttack = false;

	if (TrailComponent)
	{
		TrailComponent->Deactivate();
	}

	GetWorldTimerManager().ClearTimer(FuseTimer);

	GetWorldTimerManager().SetTimer(AttackTimer, [this]()
		{bCanAttack = true; }
	, ProjectileWeaponStat.AttackRate, false);

	AttachToOwner();
}
