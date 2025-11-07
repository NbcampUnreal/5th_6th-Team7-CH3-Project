#include "UI/InventoryUI.h"

#include "UI/ItemCategoryInfo.h"

#include "UI/HoveredDescWidgetBase.h"

void UInventoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetDataTable();
}

void UInventoryUI::OnDescActivated_Implementation(FName ItemName)
{
	if (!IsValid(Desc)) return;

	FItemSlotInfo* Info = FindItemInfo(ItemName);
	if (Info)
	{
		Desc->OnActivated(*Info);
	}
}

void UInventoryUI::OnDescDeactivated_Implementation()
{
	if (!IsValid(Desc)) return;

	Desc->OnDeactivated();
}

FItemSlotInfo* UInventoryUI::FindItemInfo(FName ItemName)
{
	if (!IsValid(ItemCategoryInfoDataTable)) return nullptr;
	if (InfoList.Num() <= 0) return nullptr;

	for (const auto& Info : InfoList)
	{
		if (Info->ItemName == ItemName)
			return Info;
	}

	return nullptr;
}

void UInventoryUI::GetDataTable()
{
	if (!IsValid(ItemCategoryInfoDataTable)) return;
	if (InfoList.Num() > 0) InfoList.Empty();

	static const FString ContextString(TEXT("ItemUIInfoContext"));

	TArray<FItemCategoryInfo*> Categories;
	ItemCategoryInfoDataTable->GetAllRows(ContextString, Categories);

	for (auto& c : Categories)
	{
		for (auto& i : c->ItemSlotInfoList)
		{
			InfoList.Add(&i);
		}
	}
}
