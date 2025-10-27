#include "UI/IngameHUD.h"

#include "UI/ReticleWidget.h"

void UIngameHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
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
