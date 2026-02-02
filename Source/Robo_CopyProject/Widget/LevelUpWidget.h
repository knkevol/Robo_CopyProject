// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BenefitWidget.h"
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

	virtual void NativeConstruct() override;

	//---------------------LevelUpScreen-----------------
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<UBorder> LevelUpScreen;
	//-------------------------------------------------------


	//---------------------LevelUpStatScreen-----------------
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<UBorder> LevelUpStatScreen;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<UBenefitWidget> Benefit01;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<UBenefitWidget> Benefit02;
	//-------------------------------------------------------

	// 초기 화면 설정 (Border1 ON, Border2 OFF)
	void InitLevelUpScreen();

	// 화면 전환 (T키 눌렀을 때 호출)
	void ToggleScreen();

	UFUNCTION()
	void HandleBenefitClicked(); // 위젯을 닫는 실제 함수

protected:
	bool bIsStatScreenValue = false;

	
};
