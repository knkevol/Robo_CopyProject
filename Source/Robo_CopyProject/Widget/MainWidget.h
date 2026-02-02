// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSettingClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnExitClicked);
/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintAssignable, Category = "RoboWidget")
	FOnStartClicked OnStartClicked;

	UPROPERTY(BlueprintAssignable, Category = "RoboWidget")
	FOnSettingClicked OnSettingClicked;

	UPROPERTY(BlueprintAssignable, Category = "RoboWidget")
	FOnExitClicked OnExitClicked;

protected:
	UFUNCTION()
	void HandleStartButton();

	UFUNCTION()
	void HandleSettingButton();

	UFUNCTION()
	void HandleExitButton();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UButton> StartButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UButton> SettingButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UButton> ExitButton;

	
};
