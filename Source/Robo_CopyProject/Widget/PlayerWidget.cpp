// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include "ProjectileWidget.h"
#include "PlayerTopWidget.h"

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerTopWidget)
	{
		SetTopWidgetVisibility(false);
	}

	if (ProjectileWidget)
	{
		ProjectileWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlayerWidget::UpdateBullet(int32 Cur, int32 Max)
{
	if (ProjectileWidget)
	{
		ProjectileWidget->SetWidgetBullet(Cur, Max);
	}
}

void UPlayerWidget::SetTopWidgetVisibility(bool bIsVisible)
{
	if (PlayerTopWidget)
	{
		ESlateVisibility NewVisibility = bIsVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
		PlayerTopWidget->SetVisibility(NewVisibility);
	}
}
