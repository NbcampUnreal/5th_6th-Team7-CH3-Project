#include "UI/PanelModeSlot.h"

#include "UI/ModeSlotButton.h"

#include "Components/UniformGridPanel.h"

#include "Blueprint/WidgetTree.h"

void UPanelModeSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!IsValid(ModeSlotClass) || !IsValid(ModeSlotInfoDataTable)) return;
	if (!IsValid(ModeSlotGrid)) return;
	if (!ModeSlotList.IsEmpty()) return;

	static const FString ContextString(TEXT("ModeSlotInfoContext"));

	TArray<FName> RowNameList = ModeSlotInfoDataTable->GetRowNames();
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
	}
}

void UPanelModeSlot::OnSelectedButtonChanged_Implementation(UModeSlotButton* NewSelected)
{
	for (auto& m : ModeSlotList)
	{
		if (m == NewSelected)
		{
			m->OnSelectedMode(m);
		}
		else
		{
			m->OnDeselectedMode();
		}
	}
}
