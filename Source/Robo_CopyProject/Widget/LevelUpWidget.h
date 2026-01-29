// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelUpWidget.generated.h"

class UBorder;
/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ULevelUpWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<UBorder> LevelUpScreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<UBorder> LevelUpStatScreen;

	// 초기 화면 설정 (Border1 ON, Border2 OFF)
	void InitLevelUpScreen();

	// 화면 전환 (T키 눌렀을 때 호출)
	void ToggleScreen();

protected:
	bool bIsStatScreenValue = false;

	
};
