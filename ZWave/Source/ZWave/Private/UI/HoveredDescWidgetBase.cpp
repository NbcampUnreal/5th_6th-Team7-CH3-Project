#include "UI/HoveredDescWidgetBase.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UHoveredDescWidgetBase::OnActivated_Implementation(const FItemUIInfo& Info)
{
	bIsActivated = true;

	if (IsValid(TitleText))
		TitleText->SetText(FText::FromString(Info.ItemName.ToString()));
	if (IsValid(DescText))
		DescText->SetText(Info.Desc);
	if (IsValid(IconImage))
	{
		if (Info.Icon.IsPending())
		{
			Info.Icon.LoadSynchronous();
		}

		UMaterialInterface* MI = Info.Icon.Get();
		if (IsValid(MI))
		{
			IconImage->SetBrushFromMaterial(MI);
		}
	}
}

void UHoveredDescWidgetBase::OnDeactivated_Implementation()
{
	bIsActivated = false;
}
