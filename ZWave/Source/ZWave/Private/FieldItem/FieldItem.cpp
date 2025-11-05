#include "FieldItem/FieldItem.h"
#include "Components/SphereComponent.h"
#include "Effect/EffectApplyManager.h"
#include "Player/TaskPlayer.h"


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
}

void AFieldItem::BeginPlay()
{
	Super::BeginPlay();

}



void AFieldItem::OnFieldItemBeingOverlap(UPrimitiveComponent* OtherlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATaskPlayer* Player = Cast<ATaskPlayer>(OtherActor))
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

