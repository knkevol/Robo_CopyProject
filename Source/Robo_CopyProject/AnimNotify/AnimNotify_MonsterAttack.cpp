// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_MonsterAttack.h"
#include "../Monster/RoboMonster.h"

FString UAnimNotify_MonsterAttack::GetNotifyName_Implementation() const
{
	return TEXT("MonsterAttack");
}

void UAnimNotify_MonsterAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ARoboMonster* Monster = Cast<ARoboMonster>(MeshComp->GetOwner());

    if (Monster)
    {
        Monster->ProcessAttackHit();
    }
}
