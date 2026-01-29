// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RoboPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ARoboPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RoboStats")
	int32 PlayerLevel = 1;

	void LevelUp();
	
};
