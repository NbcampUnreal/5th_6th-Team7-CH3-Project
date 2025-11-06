#include "UI/PanelModeSlot.h"

#include "UI/ModeSlotButton.h"
#include "UI/ItemCategoryInfo.h"
#include "UI/PanelModeDesc.h"

#include "Components/UniformGridPanel.h"

#include "Blueprint/WidgetTree.h"

void UPanelModeSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!IsValid(ModeDescPanel))
		FindPanel();

	if (!IsValid(ModeSlotClass) || !IsValid(ModeSlotInfoDataTable)) return;
	if (!IsValid(ModeSlotGrid)) return;
	if (!ModeSlotList.IsEmpty()) return;

	static const FString ContextString(TEXT("ModeSlotInfoContext"));

	FItemCategoryInfo* RowData = ModeSlotInfoDataTable->FindRow<FItemCategoryInfo>(FName("CategoryMode"), ContextString);
	for (auto& ItemData : RowData->ItemSlotInfoList)
	{
		FString Name = ItemData.ItemName.ToString() + "Mode";
		UUserWidget* Widget = WidgetTree->ConstructWidget<UUserWidget>(ModeSlotClass, FName(Name));
		if (IsValid(Widget))
		{
			UModeSlotButton* ModeSlot = Cast<UModeSlotButton>(Widget);
			if (IsValid(ModeSlot))
			{
				ModeSlot->InitModeSlot(ItemData, this);

				ModeSlotList.Add(ModeSlot);

				if (ModeSlotGrid->CanAddMoreChildren())
				{
					int row = (ModeSlotList.Num() - 1) / MODES_IN_ROW;
					int col = (ModeSlotList.Num() - 1) % MODES_IN_ROW;

					ModeSlotGrid->AddChildToUniformGrid(ModeSlot, row, col);
				}
			}
		}
	}

	/*TArray<FName> RowNameList = ModeSlotInfoDataTable->GetRowNames();
	for (auto& RowName : RowNameList)
	{
		FString Name = RowName.ToString() + "ModeSlot";
		UUserWidget* Widget = WidgetTree->ConstructWidget<UUserWidget>(ModeSlotClass, FName(Name));
		if (IsValid(Widget))
		{
			UModeSlotButton* ModeSlot = Cast<UModeSlotButton>(Widget);
			if (IsValid(ModeSlot))
			{
				ModeSlot->InitModeSlot(*ModeSlotInfoDataTable->FindRow<FModeSlotButtonInfo>(RowName, ContextString), this);

				ModeSlotList.Add(ModeSlot);

				if (ModeSlotGrid->CanAddMoreChildren())
				{
					int row = (ModeSlotList.Num() - 1) / MODES_IN_ROW;
					int col = (ModeSlotList.Num() - 1) % MODES_IN_ROW;

					ModeSlotGrid->AddChildToUniformGrid(ModeSlot, row, col);
				}
			}
		}
	}*/
}

void UPanelModeSlot::FindPanel()
{
	UPanelWidget* Parent = Cast<UPanelWidget>(GetParent()->GetParent()); // 일단 구조상 이러하니 이렇게
	if (IsValid(Parent))
	{
		TArray<UWidget*> Children = Parent->GetAllChildren();
		for (auto* c : Children)
		{
			UPanelModeDesc* Desc = Cast<UPanelModeDesc>(c);
			if (IsValid(Desc))
			{
				ModeDescPanel = Desc;
				return;
			}
		}
	}
}

void UPanelModeSlot::OnSelectedButtonChanged_Implementation(UModeSlotButton* NewSelected)
{
	for (UModeSlotButton* m : ModeSlotList)
	{
		if (m == NewSelected)
		{
			m->OnSelectedMode(m);
			ModeDescPanel->OnSelectedButtonChanged(m);
		}
		else
		{
			m->OnDeselectedMode();
		}
	}
}
