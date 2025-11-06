#include "FieldItem/FieldItem.h"
#include "Components/SphereComponent.h"
#include "Effect/EffectApplyManager.h"
#include "Player/TaskPlayer.h"
#include "Effect/HealthEffect.h"
#include "Effect/SpareAmmoAddingEffect.h"
#include "Effect/GainItemEffect.h"

AFieldItem::AFieldItem()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(Scene);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	Mesh->SetupAttachment(Collision);

	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AFieldItem::OnFieldItemBeingOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AFieldItem::OnFieldItemEndOverlap);

	SetMaps();
}

void AFieldItem::BeginPlay()
{
	Super::BeginPlay();

}



void AFieldItem::OnFieldItemBeingOverlap(UPrimitiveComponent* OtherlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor))
	{
		CauserActor = Player;
		ActiveEffect(Player);
	}

}

void AFieldItem::OnFieldItemEndOverlap(UPrimitiveComponent* OtherlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtehrBodyIndex)
{
}

void AFieldItem::Init(int32 ItemIndex)
{
	this->FieldItemIndex = ItemIndex;

	int32 Key = FieldItemIndex > FieldItemMap.Num() ? 3 : FieldItemIndex; // 재료 아이템 키 보정
	Mesh->SetStaticMesh(FieldItemMap[Key].Get<0>());
	Mesh->SetRelativeScale3D(FieldItemMap[Key].Get<1>());
}

void AFieldItem::ActiveEffect(AActor* OtherActor)
{
	if (UEffectApplyManager* EffectApplyManager = GetWorld()->GetSubsystem<UEffectApplyManager>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Activated Player"));
		EffectApplyManager->ApplyEffect(OtherActor, FieldItemEffectClassArray, 0, FieldItemIndex);
		this->CauserActor = nullptr;

		DestroyItem();
	}
}

void AFieldItem::DestroyItem()
{
	Destroy();
}

void AFieldItem::SetMaps()
{
	const ConstructorHelpers::FObjectFinder<UStaticMesh> KitMesh(TEXT("/Game/Fab/Individual_First_Aid_Kit__IFAK_Black_color_/individual_first_aid_kit_ifakblack_color.individual_first_aid_kit_ifakblack_color"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoMesh(TEXT("/Game/Fab/Ammo_box/ammobox_low.ammobox_low"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> IngrediantMesh(TEXT("/Game/Fab/Half-Life_2___Ammo_Crate/half_life_2_ammo_crate.half_life_2_ammo_crate"));

	if (KitMesh.Succeeded())
	{
		FieldItemMap.Add(1, MakeTuple(KitMesh.Object, FVector(1.0f)));
		FieldItemEffectClassArray.Add(UHealthEffect::StaticClass());
	}
	if (AmmoMesh.Succeeded())
	{
		FieldItemMap.Add(2, MakeTuple(AmmoMesh.Object, FVector(4.5f)));
		FieldItemEffectClassArray.Add(USpareAmmoAddingEffect::StaticClass());
	}
	if (IngrediantMesh.Succeeded())
	{
		FieldItemMap.Add(3, MakeTuple(IngrediantMesh.Object, FVector(0.3f)));
		FieldItemEffectClassArray.Add(UGainItemEffect::StaticClass());
	}
}

