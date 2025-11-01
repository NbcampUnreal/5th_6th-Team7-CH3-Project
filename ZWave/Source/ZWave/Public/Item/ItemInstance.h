// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemInstance.generated.h"

class UItemDefinition;

UCLASS()
class ZWAVE_API UItemInstance : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadOnly, Category = "Item Instance")
    TObjectPtr<UItemDefinition> ItemDef;

    UPROPERTY(BlueprintReadOnly, Category = "Item Instance")
    int32 StackCount = 1;

    UFUNCTION(BlueprintCallable, Category = "Item Instance")
    void Initialize(UItemDefinition* Definition, int32 Count = 1);

    UFUNCTION(BlueprintCallable, Category = "Item Instance")
    void SetStackCount(int32 NewCount);

    UFUNCTION(BlueprintCallable, Category = "Item Instance")
    int32 AddStack(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Item Instance")
    void RemoveStack(int32 Amount);

    UFUNCTION(BlueprintPure, Category = "Item Instance")
    int32 GetMaxStackCount() const;
};
