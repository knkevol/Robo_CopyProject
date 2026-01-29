// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_DeadEnd.h"
#include "../Player/RoboPlayer.h"

FString UAnimNotify_DeadEnd::GetNotifyName_Implementation() const
{
	return TEXT("DeadEnd");
}

void UAnimNotify_DeadEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_DeadEnd::Notify"));
	ARoboPlayer* RoboPlayer = Cast<ARoboPlayer>(MeshComp->GetOwner());
	if (RoboPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAnimNotify_DeadEnd::Notify_MontageEnded!!!!!!!!"));
		RoboPlayer->OnDeathMontageEnded();
	}
}
