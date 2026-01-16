// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectileWidget.generated.h"


class UTextBlock;
/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API UProjectileWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<UTextBlock> WidgetCurBullet;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWidget", meta = (WidgetBind))
	TObjectPtr<UTextBlock> WidgetMaxBullet;

	void SetWidgetBullet(int32 InCurBullet, int32 InMaxBullet);
	
};
