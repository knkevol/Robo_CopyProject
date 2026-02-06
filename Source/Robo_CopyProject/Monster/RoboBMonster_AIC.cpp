// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboBMonster_AIC.h"
#include "BrainComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "Kismet/GameplayStatics.h"

#include "../Player/RoboPlayer.h"

ARoboBMonster_AIC::ARoboBMonster_AIC()
{
	BossPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("BossPerception"));

	UAISenseConfig_Sight* Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	Sight->SightRadius = 800.0f;
	Sight->LoseSightRadius = 900.0f;
	Sight->PeripheralVisionAngleDegrees = 45.f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	BossPerception->ConfigureSense(*Sight);
	BossPerception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ARoboBMonster_AIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	ARoboBossMonster* Monster = Cast<ARoboBossMonster>(InPawn);
	if (Monster && Monster->BTAsset)
	{
		UBlackboardComponent* BlackboardComp = Blackboard.Get();
		if (UseBlackboard(Monster->BTAsset->BlackboardAsset, BlackboardComp))
		{
			RunBehaviorTree(Monster->BTAsset);
		}

	}

	//Perception->OnPerceptionUpdated.AddDynamic(this, &AZombie_AIC::ProcessPerception);
	//Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AZombie_AIC::ProcessActorPerceptionInfo);
	BossPerception->OnTargetPerceptionForgotten.AddDynamic(this, &ARoboBMonster_AIC::ProcessPerceptionForget);
	BossPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ARoboBMonster_AIC::ProcessActorPerception);
}

void ARoboBMonster_AIC::OnUnPossess()
{
	Super::OnUnPossess();
}

void ARoboBMonster_AIC::ProcessPerception(const TArray<AActor*>& UpdatedActors)
{
	for (auto Actor : UpdatedActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProcessPerception %s"), *Actor->GetName());
	}
}

void ARoboBMonster_AIC::ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed()) //ºÃÀ»¶§
		{
			//UE_LOG(LogTemp, Warning, TEXT("Type Sight"));

			ARoboPlayer* Player = Cast<ARoboPlayer>(Actor);
			ARoboBossMonster* BMonster = Cast<ARoboBossMonster>(GetPawn());
			if (Player && BMonster)
			{
				if (BMonster->GetCurrentState() == EBMonsterState::Death)
				{
					return;
				}

				Blackboard->SetValueAsObject(TEXT("Target"), Player);
				SetState(EBMonsterState::Battle);
				BMonster->SetState(EBMonsterState::Battle);
			}
		}
		else // ¸øºÃÀ»¶§
		{
			ARoboPlayer* Player = Cast<ARoboPlayer>(Actor);
			ARoboBossMonster* BMonster = Cast<ARoboBossMonster>(GetPawn());
			if (Player && BMonster)
			{
				if (BMonster->GetCurrentState() == EBMonsterState::Death)
				{
					return;
				}

				Blackboard->SetValueAsObject(TEXT("Target"), nullptr);
				SetState(EBMonsterState::Normal);
				BMonster->SetState(EBMonsterState::Normal);
			}
		}
	}
}

void ARoboBMonster_AIC::ProcessPerceptionForget(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("ProcessPerceptionForget %s"), *Actor->GetName());

	//¾Æ±î º»°÷ ±î¸ÔÀ¸¸é 
	ARoboPlayer* Player = Cast<ARoboPlayer>(Actor);
	ARoboBossMonster* BMonster = Cast<ARoboBossMonster>(GetPawn());
	if (Player && BMonster)
	{
		if (BMonster->GetCurrentState() == EBMonsterState::Death)
		{
			return;
		}

		Blackboard->SetValueAsObject(TEXT("Target"), nullptr);
		SetState(EBMonsterState::Normal);
		BMonster->SetState(EBMonsterState::Normal);
	}
}

void ARoboBMonster_AIC::SetState(EBMonsterState NewState)
{
	Blackboard->SetValueAsEnum(TEXT("CurrentState"), (uint8)(NewState));
}
