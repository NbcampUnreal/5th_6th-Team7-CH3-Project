#include "UI/ItemSlotButton.h"

#include "UI/ShopTabItem.h"

void UItemSlotButton::InitItemSlot_Implementation(FItemSlotInfo Data, UShopTabItem* Owner)
{
	ItemData = Data;
	OwnerTab = Owner;
	TargetItemName = ItemData.ItemName;
}
