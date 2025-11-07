#include "UI/ShopUI.h"

void UShopUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetWorld()->GetTimerManager().SetTimerForNextTick(
		[this]()
		{
			this->OnChangeTab(0);
		}
	);
}

void UShopUI::OnChangeTab_Implementation(int32 TargetTabIdx)
{
	if (!TabList.IsValidIndex(TargetTabIdx)) return;

	for (int i = 0; i < TabList.Num(); ++i)
	{
		if (i == TargetTabIdx)
		{
			TabList[i]->ActivateWidget();
			TabList[i]->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			TabList[i]->DeactivateWidget();
			TabList[i]->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
