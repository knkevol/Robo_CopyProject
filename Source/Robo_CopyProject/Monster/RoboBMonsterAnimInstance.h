// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RoboBossMonster.h"
#include "RoboBMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API URoboBMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterData")
	EBMonsterState CurrentState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterData")
	float Speed;
	
};
