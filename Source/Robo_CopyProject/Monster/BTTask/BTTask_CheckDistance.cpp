// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CheckDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckDistance::UBTTask_CheckDistance()
{
	NodeName = TEXT("CheckDistance");
}

EBTNodeResult::Type UBTTask_CheckDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Target")));
	UE_LOG(LogTemp, Warning, TEXT(" UBTTask_CheckDistance::ExecuteTask_Player : %s"), *Player->GetName());
	ARoboMonster* RMonster = Cast<ARoboMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (Player && RMonster)
	{
		FVector RMonsterLocation = RMonster->GetActorLocation();
		FVector PlayerLocation = Player->GetActorLocation();
		float Distance = FVector::Distance(RMonsterLocation, PlayerLocation);

		switch (TargetCondition)
		{
		case ECondition::GraterThan:
		{
			if (Distance > TargetDistance)
			{
				RMonster->SetState(TargetState);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)TargetState);
			}
		}
		break;
		case ECondition::LessThan:
		{
			if (Distance < TargetDistance)
			{
				RMonster->SetState(TargetState);
				OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)TargetState);
			}
		}
		break;
		}
	}


	return EBTNodeResult::Type();
}
