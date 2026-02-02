// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	void OnStartButtonClicked();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UButton> StartButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UButton> SettingButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UButton> ExitButton;

	
};
