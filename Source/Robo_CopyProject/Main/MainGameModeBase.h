// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API AMainGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainGameModeBase();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnStartButtonClicked();

	UFUNCTION()
	void OnSettingButtonClicked();

	UFUNCTION()
	void OnExitButtonClicked();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget")
	TSubclassOf<class UMainWidget> MainWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget")
	TObjectPtr<class UMainWidget> MainWidgetObject;
	
};
