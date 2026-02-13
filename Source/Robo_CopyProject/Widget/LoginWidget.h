// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"


class UEditableTextBox;
/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void Connect();

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UButton> ConnectButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<UEditableTextBox> IDText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<UEditableTextBox> PasswordText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget")
	TObjectPtr<UEditableTextBox> ServerText;

	void SaveData();

	UFUNCTION()
	void LoginResult(bool InResult);
	
};
