// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CmGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ACmGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACmGameModeBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget")
	TSubclassOf<class UStageNameWidget> StageNameWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget")
	TObjectPtr<class UStageNameWidget> StageNameWidgetObject;
	
};
