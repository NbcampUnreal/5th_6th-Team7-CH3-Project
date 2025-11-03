#include "UI/HoveredDescWidgetBase.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UHoveredDescWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UHoveredDescWidgetBase::OnActivated_Implementation(const FItemSlotInfo& Info)
{
	bIsActivated = true;

	if (IsValid(TitleText))
		TitleText->SetText(FText::FromString(Info.ItemName.ToString()));
	if (IsValid(DescText))
		DescText->SetText(Info.ItemDesc);
	if (IsValid(IconImage))
	{
		UTexture* Texture = Info.Icon.Get();
		if (IsValid(Texture))
		{
			if (!IsValid(IconMID))
			{
				IconMID = IconImage->GetDynamicMaterial();
			}

			if (IsValid(IconMID))
			{
				IconMID->SetTextureParameterValue(IconMIDValue, Texture);
			}
		}
	}
}

void UHoveredDescWidgetBase::OnDeactivated_Implementation()
{
	bIsActivated = false;
}
