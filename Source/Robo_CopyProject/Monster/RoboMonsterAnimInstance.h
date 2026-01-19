// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RoboMonster.h"
#include "RoboMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API URoboMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterData")
	EMonsterState CurrentState;
	
};
