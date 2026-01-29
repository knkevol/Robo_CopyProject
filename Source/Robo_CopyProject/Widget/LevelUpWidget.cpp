// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpWidget.h"
#include "Components/Border.h"

void ULevelUpWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void ULevelUpWidget::InitLevelUpScreen()
{
	bIsStatScreenValue = false;
	if (LevelUpScreen)
	{
		LevelUpScreen->SetVisibility(ESlateVisibility::Visible);
	}

	if (LevelUpStatScreen)
	{
		LevelUpStatScreen->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void ULevelUpWidget::ToggleScreen()
{
	bIsStatScreenValue = !bIsStatScreenValue;

	APlayerController* PC = GetOwningPlayer();

	if (bIsStatScreenValue) //BenefitScreen
	{
		LevelUpScreen->SetVisibility(ESlateVisibility::Collapsed);
		LevelUpStatScreen->SetVisibility(ESlateVisibility::Visible);

		if (PC)
		{
			PC->SetShowMouseCursor(true);
			FInputModeGameAndUI InputMode;
			InputMode.SetWidgetToFocus(this->TakeWidget());
			PC->SetInputMode(InputMode);
		}
	}
	else //Default
	{
		LevelUpScreen->SetVisibility(ESlateVisibility::Visible);
		LevelUpStatScreen->SetVisibility(ESlateVisibility::Collapsed);
	}
}
