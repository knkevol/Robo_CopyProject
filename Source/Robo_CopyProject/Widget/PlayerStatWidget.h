// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UImage;

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API UPlayerStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeOnInitialized() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat", meta = (WidgetBind))
	TObjectPtr<UImage> GuardianImage;

	//-------------HP------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat", meta = (WidgetBind))
	TObjectPtr<UProgressBar> PlayerHPBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat", meta = (WidgetBind))
	TObjectPtr<UTextBlock> CurHPText;

	UFUNCTION()
	void ProcessHPBar(float InPercent);
	


	//-------------Level------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat", meta = (WidgetBind))
	TObjectPtr<UProgressBar> PlayerXP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStat", meta = (WidgetBind))
	TObjectPtr<UTextBlock> PlayerLevel;
};
