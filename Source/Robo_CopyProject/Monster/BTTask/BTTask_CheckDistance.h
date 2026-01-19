// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "../RoboMonster.h"
#include "BTTask_CheckDistance.generated.h"

UENUM(BlueprintType)
enum class ECondition : uint8
{
	LessThan = 0  UMETA(DisplayName = "<"),
	GraterThan = 1  UMETA(DisplayName = ">"),
};

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API UBTTask_CheckDistance : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_CheckDistance();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCondition")
	ECondition TargetCondition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCondition")
	EMonsterState TargetState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCondition")
	float TargetDistance;

	
};
