// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWidget.h"
#include "Components/TextBlock.h"

void UProjectileWidget::SetWidgetBullet(int32 InCurBullet, int32 InMaxBullet)
{
	if (WidgetCurBullet && WidgetMaxBullet)
	{
		FString cur = FString::Printf(TEXT("%d"), InCurBullet);
		WidgetCurBullet->SetText(FText::FromString(cur));

		FString max = FString::Printf(TEXT("%d"), InMaxBullet);
		WidgetMaxBullet->SetText(FText::FromString(max));
	}
}
