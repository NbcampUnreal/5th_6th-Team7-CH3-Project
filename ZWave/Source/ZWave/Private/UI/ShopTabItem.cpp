#include "UI/ShopTabItem.h"

#include "UI/ItemCategory.h"
#include "UI/ItemSlotButton.h"
#include "UI/PanelItemDesc.h"

#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"

#include "Blueprint/WidgetTree.h"

void UShopTabItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!IsValid(ItemDescPanel))
		FindPanel();

	if (!IsValid(CategoryClass) || !IsValid(CategoryInfoDataTable)) return;
	if (!IsValid(ItemCategoryVB)) return;
	if (!Categories.IsEmpty()) return;

	static const FString ContextString(TEXT("CategoryInfoContext"));

	TArray<FName> RowNameList = CategoryInfoDataTable->GetRowNames();
	for (auto& RowName : RowNameList)
	{
		FString Name = RowName.ToString() + "Category";
		UUserWidget* Widget = WidgetTree->ConstructWidget<UUserWidget>(CategoryClass, FName(Name));
		if (IsValid(Widget))
		{
			UItemCategory* Category = Cast<UItemCategory>(Widget);
			if (IsValid(Category))
			{
				ItemCategoryVB->AddChildToVerticalBox(Category);
				Category->InitCategory(*CategoryInfoDataTable->FindRow<FItemCategoryInfo>(RowName, ContextString), this);

				Categories.Add(Category);
			}
		}
	}
}

void UShopTabItem::FindPanel()
{
	UPanelWidget* Parent = Cast<UPanelWidget>(MainHB);
	if (IsValid(Parent))
	{
		TArray<UWidget*> Children = Parent->GetAllChildren();
		for (auto* c : Children)
		{
			UPanelItemDesc* Desc = Cast<UPanelItemDesc>(c);
			if (IsValid(Desc))
			{
				ItemDescPanel = Desc;
				return;
			}
		}
	}
}

void UShopTabItem::OnSelectedButtonChange_Implementation(UItemSlotButton* NewSelected)
{
	for (UItemSlotButton* b : Buttons)
	{
		if (b == NewSelected)
		{
			b->OnSelectedItem(b);
			ItemDescPanel->OnSelectedButtonChanged(b);
		}
		else
		{
			b->OnDeselectedItem();
		}
	}
}

void UShopTabItem::AddButtons(TArray<UItemSlotButton*> NewButtons)
{
	Buttons.Append(NewButtons);
}
