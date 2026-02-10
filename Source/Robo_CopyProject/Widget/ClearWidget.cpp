// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearWidget.h"
#include "Components/Button.h"

void UClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UClearWidget::HandleContinueButton);
	}
}

void UClearWidget::HandleContinueButton()
{
	OnContinueClicked.Broadcast();
}
