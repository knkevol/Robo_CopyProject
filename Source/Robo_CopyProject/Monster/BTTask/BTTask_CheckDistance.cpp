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
	if (!Player)
	{
		//UE_LOG(LogTemp, Error, TEXT("UBTTask_CheckDistance: Player (Target) is Null!"));
		return EBTNodeResult::Failed;
	}

	//UE_LOG(LogTemp, Warning, TEXT("UBTTask_CheckDistance::ExecuteTask_Player : %s"), *Player->GetName());
	ARoboMonster* RMonster = Cast<ARoboMonster>(OwnerComp.GetAIOwner()->GetPawn());
	ARoboBossMonster* BMonster = Cast<ARoboBossMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (Player && (RMonster || BMonster))
	{
		FVector MonsterLocation;
		if (RMonster)
		{
			MonsterLocation = RMonster->GetActorLocation();
		}
		else
		{
			MonsterLocation = BMonster->GetActorLocation();
		}
		
		FVector PlayerLocation = Player->GetActorLocation();
		float Distance = FVector::Distance(MonsterLocation, PlayerLocation);

		switch (TargetCondition)
		{
		case ECondition::GraterThan:
		{
			if (Distance > TargetDistance)
			{
				if (RMonster)
				{
					RMonster->SetState(TargetState);
					OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)TargetState);
				}
				else
				{
					BMonster->SetState(TargetBState);
					OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)TargetBState);
				}
				
			}
		}
		break;
		case ECondition::LessThan:
		{
			if (Distance < TargetDistance)
			{
				if (RMonster)
				{
					RMonster->SetState(TargetState);
					OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)TargetState);
				}
				else
				{
					BMonster->SetState(TargetBState);
					OwnerComp.GetBlackboardComponent()->SetValueAsEnum(GetSelectedBlackboardKey(), (uint8)TargetBState);
				}
			}
		}
		break;
		}
	}


	return EBTNodeResult::Type();
}
