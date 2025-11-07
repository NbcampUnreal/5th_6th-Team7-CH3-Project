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
#include <NiagaraFunctionLibrary.h>
#include "Engine/OverlapResult.h"
#include "DamageCalculator/DamageCalculator.h"
#include "DrawDebugHelpers.h"
#include "AoE/AoESpawningManager.h"
#include "Enemy/RangedAIController.h"

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
	ProjectileMovement->MaxSpeed = 10000.f;
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

	ProjectileWeaponStatBase = ProjDefinition->ProjectileWeaponStat;
	ProjectileWeaponStat = ProjDefinition->ProjectileWeaponStat;
	ProjectileMovement->InitialSpeed = ProjectileWeaponStat.InitialSpeed;
	ProjectileMovement->SetActive(false);
	AttachSocketName = ProjDefinition->WeaponAttachSocket;

	SphereCollision->IgnoreActorWhenMoving(GetOwner(), true);
	AttachToOwner();

    return true;
}

bool AProjectileWeapon::EquipModing(EModingSlot ModingSlot, UModingInstance* ModeInstance)
{
	if (ModeInstance == nullptr)
		return false;

	const UProjectileWeaponDefinition* PrjectileDefinition = Cast<UProjectileWeaponDefinition>(ModeInstance->GetModeWeaponDef());
	if (PrjectileDefinition == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("It's Not ShootWeapon Moding!"));
		return false;
	}

	if (EquipModingMap.Contains(ModingSlot))
	{
		if (EquipModingMap[ModingSlot] != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Need UnEquip Slot"));
			return false;
		}
	}

	int32 EquipNum = EquipModingMap.Num();
	if (EquipModingMap.Contains(EModingSlot::EMS_Default))
		EquipNum--;

	EquipModingMap.Add(ModingSlot, ModeInstance);
	EquipModingEffectClassMap.Add(ModingSlot, ModeInstance->GetModeEffectClass());
	ApplyCurrentModing();
	return true;
}

void AProjectileWeapon::UnEquipModing(EModingSlot ModingSlot)
{
	if (EquipModingMap.Contains(ModingSlot) == false)
	{
		return;
	}

	EquipModingMap.Remove(ModingSlot);
	EquipModingEffectClassMap.Remove(ModingSlot);
	ApplyCurrentModing();
}

void AProjectileWeapon::ApplyCurrentModing()
{
	FProjectileWeaponStats ProjectileStat = ProjectileWeaponStatBase;

	for (const auto& Pair : EquipModingMap)
	{
		UModingInstance* ModingIns = Pair.Value;
		if (ModingIns == nullptr)
			continue;

		UProjectileWeaponDefinition* ProjectileDef = Cast<UProjectileWeaponDefinition>(ModingIns->GetModeWeaponDef());
		if (ProjectileDef == nullptr)
			continue;

		FProjectileWeaponStats& ModingStat = ProjectileDef->ProjectileWeaponStat;
		EWeaponModifier ModifierType = ModingIns->GetModeApplyType();

		ApplyStat(ModingStat, ModifierType, ProjectileStat);
	}

	ProjectileWeaponStat = ProjectileStat;
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

	if (const ARangedAIController* RAC = Cast<ARangedAIController>(Controller))
	{
		FVector TargetVec;
		RAC->GetTargetVector(TargetVec);

		DrawDebugLine(GetWorld(), TargetVec, PawnOwner->GetActorLocation(), FColor::Red, false, 0.2f, 0, 1.5f);
		const FVector Dir = (TargetVec - PawnOwner->GetActorLocation()).GetSafeNormal();
		return Dir;
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
		TrailComponent->ReinitializeSystem();
	}

	// 안보이게 설정
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void AProjectileWeapon::ApplyStat(const FProjectileWeaponStats& ModingStat, EWeaponModifier ModifierType, FProjectileWeaponStats& OutStat)
{
	switch (ModifierType)
	{
	case EWeaponModifier::EWM_Add:
	{
		OutStat.AttackPower += ModingStat.AttackPower;
		OutStat.AttackRate += ModingStat.AttackRate;
		OutStat.Radius += ModingStat.Radius;
		OutStat.FuseTime += ModingStat.FuseTime;
		OutStat.InitialSpeed += ModingStat.InitialSpeed;
	}
	break;
	case EWeaponModifier::EWM_Percent:
	{
		OutStat.AttackPower *= (1.0f + ModingStat.AttackPower);
		OutStat.AttackRate *= (1.0f + ModingStat.AttackRate);
		OutStat.Radius *= (1.0f + ModingStat.Radius);
		OutStat.FuseTime *= (1.0f + ModingStat.FuseTime);
		OutStat.InitialSpeed *= (1.0f + ModingStat.InitialSpeed);
	}
	break;
	case EWeaponModifier::EWM_Multiple:
	{
		OutStat.AttackPower *= ModingStat.AttackPower;
		OutStat.AttackRate *= ModingStat.AttackRate;
		OutStat.Radius *= ModingStat.Radius;
		OutStat.FuseTime *= ModingStat.FuseTime;
		OutStat.InitialSpeed *= ModingStat.InitialSpeed;
	}
	break;
	}
}

void AProjectileWeapon::OnProjectileStop(const FHitResult& ImpactResult)
{
	HitLocation = ImpactResult.ImpactPoint;

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


void AProjectileWeapon::DamageBoom()
{
	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	FCollisionObjectQueryParams ObjQuery;
	ObjQuery.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(GrenadeOverlap), false);
	QueryParams.AddIgnoredActor(this);
	if (AActor* OwnerActor = GetOwner())
	{
		QueryParams.AddIgnoredActor(OwnerActor);
		QueryParams.AddIgnoredActor(this);
	}

	TArray<FOverlapResult> Overlaps;
	const bool bHit = World->OverlapMultiByObjectType(
		Overlaps,
		GetActorLocation(),
		FQuat::Identity,
		ObjQuery,
		FCollisionShape::MakeSphere(ProjectileWeaponStat.Radius),
		QueryParams
	);

	if (bHit)
	{
		for (const FOverlapResult& R : Overlaps)
		{
			AActor* Other = R.GetActor();

			if (Other == nullptr
				|| Other == this
				|| Other == GetOwner())
				continue;

			OverlappedEnemies.AddUnique(Other);
		}
	}

	for (TObjectPtr<AActor> TargetActor : OverlappedEnemies)
	{
		if (IsDamagableActor(TargetActor) == true)
		{
			FZWaveDamageEvent DamageEvent;
			DamageEvent.BaseDamage = ProjectileWeaponStat.AttackPower;
			DamageEvent.Duration = 0.0f;

			TArray<TSubclassOf<UEffectBase>> EffectClasses;
			EquipModingEffectClassMap.GenerateValueArray(EffectClasses);
			DamageEvent.EffectArray = EffectClasses;
			UDamageCalculator::DamageHelper(GetWorld(), TargetActor, GetOwner(), DamageEvent);
		}
	}
}

void AProjectileWeapon::Explode()
{
	// 해당 AttackPower 부분은 DamageCalculator와 연계 예정
	DamageBoom();

	if (UAoESpawningManager* AoeManager = GetWorld()->GetSubsystem<UAoESpawningManager>())
	{
		AoeManager->SpawnAoEActor(GetOwner(), ProjectileWeaponStat.ProjectileIdx, HitLocation);
	}

	bCanAttack = false;

	if (ProjectileWeaponStat.ExplosionVFX) // UNiagaraSystem* 변수
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ProjectileWeaponStat.ExplosionVFX,
			GetActorLocation(),
			GetActorRotation(),
			FVector(1.0),   // 스케일
			true,           // 자동 파괴
			true,           // 풀 체크(옵션)
			ENCPoolMethod::AutoRelease
		);
	}

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
