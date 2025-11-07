// Fill out your copyright notice in the Description page of Project Settings.
#include "Effect/GainItemEffect.h"
#include "Item/InventoryComponent.h"
#include "Level/ItemSpawnManager.h"
#include "Item/ItemDefinition.h"

UGainItemEffect::UGainItemEffect()
{

}


void UGainItemEffect::ApplyEffect(AActor* TargetActor, const float& Duration)
{
	
}

void UGainItemEffect::ApplyEffect(AActor* TargetActor, const int32& Itemindex)
{
	if (ATaskPlayer* Player = Cast<ATaskPlayer>(TargetActor))
	{
		if (UInventoryComponent* InvenComp = Player->FindComponentByClass<UInventoryComponent>())
		{
			if (UObject* Outer = GetOuter())
			{
				if (UWorld* World = Outer->GetWorld())
				{
					if (UItemSpawnManager* ItemSpawnManager = World->GetSubsystem<UItemSpawnManager>())
					{
						if (UItemDefinition* Def = ItemSpawnManager->GetItemDefinitionByIndex(Itemindex))
						{
							InvenComp->AddItem(Def, 1);
						}
					}
				}
			}
		}

		RemoveEffect();
	}
}

void UGainItemEffect::RemoveEffect()
{
	Super::RemoveEffect();
}

void UGainItemEffect::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTemp, Warning, TEXT("UGainItemEffect has been successfully collected by GC."));
}


