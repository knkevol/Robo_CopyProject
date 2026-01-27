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
        // 2. 몬스터 내부에 만들어둔 "공격 판정 함수"를 호출합니다.
        // 이 함수 내부에서 Sweep/LineTrace를 통해 플레이어를 찾고 ApplyDamage를 호출합니다.
        Monster->ProcessAttackHit();
    }
}
