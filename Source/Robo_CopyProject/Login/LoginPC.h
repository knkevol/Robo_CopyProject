// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LoginPC.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ALoginPC : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:

	virtual void PostInitializeComponents() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboLoginPC")
	TSubclassOf<class ULoginWidget> LoginWidgetClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "RoboLoginPC")
	TObjectPtr<class ULoginWidget> LoginWidgetObject;
};
