// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerTopWidget.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API UPlayerTopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UTextBlock> BossNameText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<class UProgressBar> BossHPBar;

	void UpdateBossInfo(class ARoboBossMonster* TargetBoss);

	UFUNCTION()
	void ProcessBossHPBar(float InPercent);
	
};
