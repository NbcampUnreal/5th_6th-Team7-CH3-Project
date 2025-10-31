#include "UI/ShopTabEnforce.h"

#include "UI/EnforceMenu.h"

FOnClickPlusButton* UShopTabEnforce::FindDelegate(FName RowName)
{
	FOnClickPlusButton* Target = nullptr;
	for (auto& m : Menus)
	{
		if (m->GetName() == RowName)
		{
			Target = &m->GetDelegate();
			break;
		}
	}

	return Target;
}
