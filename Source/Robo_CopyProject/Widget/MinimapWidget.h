// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapWidget.generated.h"

class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UImage> PMinimapImage;

	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintCallable)
	void SetMinimapImage(UMaterialInstanceDynamic* InMI);
};
