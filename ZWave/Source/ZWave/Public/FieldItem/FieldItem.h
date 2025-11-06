// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FieldItem.generated.h"

UCLASS()
class ZWAVE_API AFieldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AFieldItem();

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	class USphereComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TArray<TSubclassOf<class UEffectBase>> FieldItemEffectClassArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	AActor* CauserActor;

	UFUNCTION(BlueprintCallable, Category = "Initalize")
	void Init(int32 ItemIndex);

protected:
	UFUNCTION(BlueprintCallable, Category = "Event")
	virtual void OnFieldItemBeingOverlap(
		UPrimitiveComponent* OtherlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComponent, 
		int32 OtherBodyIndex, 
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Event")
	virtual void OnFieldItemEndOverlap(
		UPrimitiveComponent* OtherlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtehrBodyIndex
	);

	UFUNCTION(BlueprintCallable, Category = "Event")
	virtual void ActiveEffect(AActor* OtherActor);

	virtual void DestroyItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Index")
	int32 FieldItemIndex;

private:
	TMap<int32, TTuple<UStaticMesh*, FVector>> FieldItemMap;

	void SetMaps();

};
