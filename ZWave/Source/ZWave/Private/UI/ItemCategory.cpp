#include "UI/ItemCategory.h"

#include "UI/ItemSlotInfo.h"
#include "UI/ItemSlotButton.h"
#include "UI/ShopTabItem.h"

#include "Components/UniformGridPanel.h"

#include "Blueprint/WidgetTree.h"

void UItemCategory::InitCategory_Implementation(FItemCategoryInfo Data, UShopTabItem* Parent)
{
	if (!IsValid(ItemSlotButtonClass)) return;
	if (!IsValid(ItemSlotButtonGrid)) return;
	if (!ItemSlotButtons.IsEmpty()) return;

	for (FItemSlotInfo& i : Data.ItemSlotInfoList)
	{
		FString Name = i.ItemName.ToString() + "Slot";
		UUserWidget* Widget = WidgetTree->ConstructWidget<UUserWidget>(ItemSlotButtonClass, FName(Name));
		if (IsValid(Widget))
		{
			UItemSlotButton* ItemSlot = Cast<UItemSlotButton>(Widget);
			if (IsValid(ItemSlot))
			{
				ItemSlotButtons.Add(ItemSlot);

				int row = (ItemSlotButtons.Num() - 1) / MODES_IN_ROW;
				int col = (ItemSlotButtons.Num() - 1) % MODES_IN_ROW;

				ItemSlotButtonGrid->AddChildToUniformGrid(ItemSlot, row, col);

				ItemSlot->InitItemSlot(i, Parent);
			}
		}
	}

	ParentTab = Parent;
	CategoryData = Data;
	ParentTab->AddButtons(ItemSlotButtons);
}
