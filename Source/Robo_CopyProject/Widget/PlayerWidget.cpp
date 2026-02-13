// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include "ProjectileWidget.h"
#include "PlayerTopWidget.h"

#include "Kismet/GameplayStatics.h"
#include "../Monster/RoboBossMonster.h"

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerTopWidget)
	{
		SetTopWidgetVisibility(false);
	}

	if (ProjectileWidget)
	{
		//ProjectileWidget->SetVisibility(ESlateVisibility::Collapsed);
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
		if (bIsVisible)
		{
			PlayerTopWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			PlayerTopWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}

void UPlayerWidget::SetTopWidgetVisibility(bool bIsVisible, ARoboBossMonster* SpawnedBoss)
{
	if (PlayerTopWidget)
	{
		if (bIsVisible)
		{
			if (SpawnedBoss)
			{
				SpawnedBoss->CachedTopWidget = PlayerTopWidget;
				PlayerTopWidget->UpdateBossInfo(SpawnedBoss);
				PlayerTopWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			PlayerTopWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
