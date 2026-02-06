// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RoboBossMonster.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "RoboBMonster_AIC.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ARoboBMonster_AIC : public AAIController
{
	GENERATED_BODY()

public:
	ARoboBMonster_AIC();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "RoboData")
	TObjectPtr<UAIPerceptionComponent> BossPerception;

	UFUNCTION()
	void ProcessPerception(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void ProcessPerceptionForget(AActor* Actor);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<UBehaviorTree> RunBTAsset;

	void SetState(EBMonsterState NewState);

	
};
