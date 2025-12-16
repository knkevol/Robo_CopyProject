// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoboPlayerController.generated.h"

class UPlayerWidget;

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ARoboPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWidget")
	TSubclassOf<UPlayerWidget> PlayerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWidget")
	TObjectPtr<UPlayerWidget> PlayerWidgetObject;
	
};
