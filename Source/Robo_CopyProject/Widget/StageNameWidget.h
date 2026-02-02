// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageNameWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API UStageNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void HideStageNameWidget();

	void StageNameTyping(FString TargetText, float TypingSpeed = 0.1f);

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UTextBlock> StageNameText;

	//--------------StageNameEffect-------------
	// 전체 문장을 담아둘 변수
	FString FullText;

	// 현재까지 출력된 글자 수
	int32 CurrentCharIndex;

	// 타이머 핸들
	FTimerHandle TypingTimerHandle;

	void OnTypingTick();
	//------------------------------------------
	
};
