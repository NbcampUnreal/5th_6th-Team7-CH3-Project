// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CeilingPoint.generated.h"

class UBoxComponent;

UCLASS()
class ZWAVE_API ACeilingPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACeilingPoint();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root")
	UBoxComponent* BoxRoot;
};
