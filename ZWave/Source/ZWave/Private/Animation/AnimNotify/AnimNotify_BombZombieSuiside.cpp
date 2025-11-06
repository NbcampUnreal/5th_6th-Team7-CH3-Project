// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_BombZombieSuiside.h"

#include "Enemy/BombZombie.h"

void UAnimNotify_BombZombieSuiside::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp == nullptr) return;

	ABombZombie* MyZombie = static_cast<ABombZombie*>(MeshComp->GetOwner());
	if (MyZombie == nullptr) return;

	MyZombie->Suiside();
}
