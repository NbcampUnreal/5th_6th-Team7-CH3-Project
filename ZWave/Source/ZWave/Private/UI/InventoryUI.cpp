#include "UI/InventoryUI.h"

#include "UI/HoveredDescWidgetBase.h"

void UInventoryUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	GetDataTable();
}

void UInventoryUI::OnDescActivated_Implementation(int32 ID)
{
	if (!IsValid(Desc)) return;

	FItemUIInfo* Info = FindItemInfo(ID);
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

FItemUIInfo* UInventoryUI::FindItemInfo(int32 ID)
{
	if (!IsValid(ItemUIInfoDataTable)) return nullptr;
	if (InfoList.Num() <= 0) return nullptr;

	for (const auto& Info : InfoList)
	{
		if (Info->ItemID == ID)
			return Info;
	}

	return nullptr;
}

void UInventoryUI::GetDataTable()
{
	if (!IsValid(ItemUIInfoDataTable)) return;
	if (InfoList.Num() > 0) InfoList.Empty();

	static const FString ContextString(TEXT("ItemUIInfoContext"));

	ItemUIInfoDataTable->GetAllRows(ContextString, InfoList);
}
