// Fill out your copyright notice in the Description page of Project Settings.


#include "BenefitWidget.h"

void UBenefitWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	SetRenderScale(FVector2D(1.1f, 1.1f));
}

void UBenefitWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	SetRenderScale(FVector2D(1.0f, 1.0f));
}

FReply UBenefitWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	OnBenefitClicked.Broadcast();
	return FReply::Handled();
}
