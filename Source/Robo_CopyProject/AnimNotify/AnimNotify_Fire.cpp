// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Fire.h"

FString UAnimNotify_Fire::GetNotifyName_Implementation() const
{
	return TEXT("Fire");
}

void UAnimNotify_Fire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
}
