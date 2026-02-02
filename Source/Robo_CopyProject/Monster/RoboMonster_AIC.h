// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RoboMonster.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "RoboMonster_AIC.generated.h"


class UAIPerceptionComponent;
class UBehaviorTree;
/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ARoboMonster_AIC : public AAIController
{
	GENERATED_BODY()

public:
	ARoboMonster_AIC();

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "RoboData")
	TObjectPtr<UAIPerceptionComponent> Perception;

	UFUNCTION()
	void ProcessPerception(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	void ProcessActorPerception(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void ProcessPerceptionForget(AActor* Actor);

	UFUNCTION()
	void ProcessActorPerceptionInfo(const FActorPerceptionUpdateInfo& UpdateInfo);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<UBehaviorTree> RunBTAsset;

	void SetState(EMonsterState NewState);
};
