// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ClearWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnContinueClicked);
/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API UClearWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;


	UPROPERTY(BlueprintAssignable, Category = "RoboWidget")
	FOnContinueClicked OnContinueClicked;

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UButton> ContinueButton;

	UFUNCTION()
	void HandleContinueButton();

	
};
