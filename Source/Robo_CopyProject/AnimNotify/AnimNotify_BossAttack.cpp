// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_BossAttack.h"
#include "../Monster/RoboBossMonster.h"

FString UAnimNotify_BossAttack::GetNotifyName_Implementation() const
{
	return TEXT("BossAttack");
}

void UAnimNotify_BossAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    ARoboBossMonster* BMonster = Cast<ARoboBossMonster>(MeshComp->GetOwner());

    if (BMonster)
    {
        //BMonster->ProcessAttackHit();
    }
}
