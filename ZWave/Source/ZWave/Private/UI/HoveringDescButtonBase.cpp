#include "UI/HoveringDescButtonBase.h"

#include "UI/InventoryUI.h"

#include "CommonUITypes.h"
#include "CommonUILibrary.h"

void UHoveringDescButtonBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UInventoryUI* Root = Cast<UInventoryUI>(UCommonUILibrary::FindParentWidgetOfType(this, UInventoryUI::StaticClass()));
	if (IsValid(Root))
	{
		Inventory = Root;
	}
}

void UHoveringDescButtonBase::OnHoveringIn_Implementation()
{
	if (!IsValid(Inventory)) return;

	Inventory->OnDescActivated(TargetItemName);
}

void UHoveringDescButtonBase::OnHoveringOut_Implementation()
{
	if (!IsValid(Inventory)) return;

	Inventory->OnDescDeactivated();
}
