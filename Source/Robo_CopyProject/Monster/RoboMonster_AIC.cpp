// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboMonster_AIC.h"
#include "RoboMonster.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "../Player/RoboPlayer.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


ARoboMonster_AIC::ARoboMonster_AIC()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception"));

	UAISenseConfig_Sight* Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));
	Sight->SightRadius = 800.0f;
	Sight->LoseSightRadius = 900.0f;
	Sight->PeripheralVisionAngleDegrees = 45.f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ARoboMonster_AIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (RunBTAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("ARoboMonster_AIC::OnPossess"));
		RunBehaviorTree(RunBTAsset);
	}

	//Perception->OnPerceptionUpdated.AddDynamic(this, &AZombie_AIC::ProcessPerception);
	//Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &AZombie_AIC::ProcessActorPerceptionInfo);
	Perception->OnTargetPerceptionForgotten.AddDynamic(this, &ARoboMonster_AIC::ProcessPerceptionForget);
	Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ARoboMonster_AIC::ProcessActorPerception);
	SetGenericTeamId(3);
}

void ARoboMonster_AIC::OnUnPossess()
{
	Super::OnUnPossess();
}

void ARoboMonster_AIC::ProcessPerception(const TArray<AActor*>& UpdatedActors)
{
	for (auto Actor : UpdatedActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("ProcessPerception %s"), *Actor->GetName());
	}
}

void ARoboMonster_AIC::ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed()) //ºÃÀ»¶§
		{
			//UE_LOG(LogTemp, Warning, TEXT("Type Sight"));

			ARoboPlayer* Player = Cast<ARoboPlayer>(Actor);
			ARoboMonster* RMonster = Cast<ARoboMonster>(GetPawn());
			if (Player && RMonster)
			{
				if (RMonster->GetCurrentState() == EMonsterState::Death)
				{
					return;
				}

				Blackboard->SetValueAsObject(TEXT("Target"), Player);
				SetState(EMonsterState::Chase);
				RMonster->SetState(EMonsterState::Chase);
				RMonster->ChangeSpeed(400.0f);
			}
		}
		else // ¸øºÃÀ»¶§
		{
			ARoboPlayer* Player = Cast<ARoboPlayer>(Actor);
			ARoboMonster* RMonster = Cast<ARoboMonster>(GetPawn());
			if (Player && RMonster)
			{
				if (RMonster->GetCurrentState() == EMonsterState::Death)
				{
					return;
				}

				Blackboard->SetValueAsObject(TEXT("Target"), nullptr);
				SetState(EMonsterState::Normal);
				RMonster->SetState(EMonsterState::Normal);
				RMonster->ChangeSpeed(80.0f);
			}
		}
	}
}

void ARoboMonster_AIC::ProcessPerceptionForget(AActor* Actor)
{
	UE_LOG(LogTemp, Warning, TEXT("ProcessPerceptionForget %s"), *Actor->GetName());

	//¾Æ±î º»°÷ ±î¸ÔÀ¸¸é 
	ARoboPlayer* Player = Cast<ARoboPlayer>(Actor);
	ARoboMonster* RMonster = Cast<ARoboMonster>(GetPawn());
	if (Player && RMonster)
	{
		if (RMonster->GetCurrentState() == EMonsterState::Death)
		{
			return;
		}

		Blackboard->SetValueAsObject(TEXT("Target"), nullptr);
		SetState(EMonsterState::Normal);
		RMonster->SetState(EMonsterState::Normal);
		RMonster->ChangeSpeed(80.0f);
	}
}

void ARoboMonster_AIC::ProcessActorPerceptionInfo(const FActorPerceptionUpdateInfo& UpdateInfo)
{
}

void ARoboMonster_AIC::SetState(EMonsterState NewState)
{
	Blackboard->SetValueAsEnum(TEXT("CurrentState"), (uint8)(NewState));
}
