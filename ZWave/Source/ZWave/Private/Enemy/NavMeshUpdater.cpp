// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/NavMeshUpdater.h"

#include "Components/BoxComponent.h"
#include "NavigationSystem.h"

// Sets default values
ANavMeshUpdater::ANavMeshUpdater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
}

// Called when the game starts or when spawned
void ANavMeshUpdater::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMeshComp != nullptr) {
		StaticMeshComp->OnComponentHit.AddDynamic(this, &ANavMeshUpdater::HandleHit);
	}
	
}


// Called every frame
void ANavMeshUpdater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANavMeshUpdater::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSystem)
	{
		UE_LOG(LogTemp, Display, TEXT("on hit rebuild %d"), NavSystem->IsNavigationUpdateLocked());
		NavSystem->Build(); // 네비메시 리빌드
	}
}

