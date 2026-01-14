// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Reload.h"
#include "../Player/RoboPlayer.h"

FString UAnimNotify_Reload::GetNotifyName_Implementation() const
{
	return TEXT("Reload");
}

void UAnimNotify_Reload::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ARoboPlayer* RoboPlayer = Cast<ARoboPlayer>(MeshComp->GetOwner());
	if (RoboPlayer)
	{
		RoboPlayer->ReloadWeapon();
	}
}
