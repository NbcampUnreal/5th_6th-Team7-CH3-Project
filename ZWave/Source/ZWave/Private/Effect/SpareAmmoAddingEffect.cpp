#include "Effect/SpareAmmoAddingEffect.h"
#include "Weapon/EquipComponent.h"
#include "Weapon/ShootWeapon.h"

USpareAmmoAddingEffect::USpareAmmoAddingEffect()
{
	AddingPercent = 0.3f;
}

void USpareAmmoAddingEffect::ApplyEffect(AActor* TargetActor, const float& BaseDamage)
{
	// TODO : 
	// EquipComponent 혹은 Inventory에서 현재 장착중인 무기에 접근
	// 무기의 SpareAmmo 값을 현재 SpareAmmo += SpareAmmo * AddingPercent
	// AddingPerecnt -> 모드(50%), 필드 아이템(30%)

	if (ABaseCharacter* Character = Cast<ABaseCharacter>(TargetActor))
	{
		if (UEquipComponent* EquipComp = Cast<UEquipComponent>(Character->GetComponentByClass<UEquipComponent>()))
		{
			// 이 부분부터 수정 될 가능성 있음
			if (AShootWeapon* CurWeapon = Cast<AShootWeapon>(EquipComp->GetCurrentWeapon()))
			{
				int32 RemainAmmo = CurWeapon->GetRemainSpareAmmo();
				RemainAmmo += RemainAmmo * AddingPercent;
				EquipComp->AmmoSupply(RemainAmmo);
			}
		}
	}
}

void USpareAmmoAddingEffect::RemoveEffect()
{
	Super::RemoveEffect();
}

void USpareAmmoAddingEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("USpareAmmoAddingEffect has been successfully collected by GC."));
}
