// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/AnimNotify_EnemyDeath.h"

void UAnimNotify_EnemyDeath::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp) 
	{
		AActor * Owner = MeshComp->GetOwner();
		if (Owner) 
		{
			Owner->SetLifeSpan(0.05f);
		}
	}
}
