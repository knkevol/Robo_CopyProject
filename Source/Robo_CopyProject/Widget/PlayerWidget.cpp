// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include "ProjectileWidget.h"

void UPlayerWidget::UpdateBullet(int32 Cur, int32 Max)
{
	if (ProjectileWidget)
	{
		ProjectileWidget->SetWidgetBullet(Cur, Max);
	}
}
