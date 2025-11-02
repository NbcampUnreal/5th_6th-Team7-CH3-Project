#include "UI/ShopTabEnforce.h"

#include "UI/EnforceMenu.h"

#include "CommonUI/Public/CommonHierarchicalScrollBox.h"

#include "Blueprint/WidgetTree.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UShopTabEnforce::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!IsValid(MenuClass) || !IsValid(EnforceInfoDataTable)) return;
	if (!IsValid(MenuScrollBox)) return;
	if (!Menus.IsEmpty()) return;

	static const FString ContextString(TEXT("EnforceInfoContext"));

	TArray<FName> RowNameList = EnforceInfoDataTable->GetRowNames();
	for (auto& RowName : RowNameList)
	{
		FString Name = RowName.ToString() + "Menu";
		UUserWidget* Widget = WidgetTree->ConstructWidget<UUserWidget>(MenuClass, FName(Name));
		if (IsValid(Widget))
		{
			UEnforceMenu* Enforce = Cast<UEnforceMenu>(Widget);
			if (IsValid(Enforce))
			{
				MenuScrollBox->AddChild(Enforce);
				Enforce->InitMenu(*EnforceInfoDataTable->FindRow<FEnforceInfo>(RowName, ContextString));

				Menus.Add(Enforce);
			}
		}
	}

}

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
