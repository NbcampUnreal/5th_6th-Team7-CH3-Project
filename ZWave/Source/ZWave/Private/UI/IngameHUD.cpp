#include "UI/IngameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Base/ZWaveGameState.h"
#include "UI/ReticleWidget.h"

void UIngameHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	AZWaveGameState* ZWaveGameState = Cast<AZWaveGameState>(UGameplayStatics::GetGameState(this));
	if (ZWaveGameState)
	{
		ZWaveGameState->RegisterIngameHUD(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("IngameHUD: Could not find ZWaveGameState on Initialize!"));
	}
}

void UIngameHUD::OnFireWeapon_Implementation(AActor* HitActor)
{
	if (IsValid(CurrentReticle))
	{
		CurrentReticle->OnFireWeapon(HitActor);
	}
}

void UIngameHUD::OnReloadWeapon_Implementation(float ReloadTime)
{
	if (IsValid(CurrentReticle))
	{
		CurrentReticle->OnReloadWeapon(ReloadTime);
	}
}
