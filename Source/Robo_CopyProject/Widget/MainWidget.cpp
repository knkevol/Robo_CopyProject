// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainWidget::HandleStartButton);
	}

	if (SettingButton)
	{
		SettingButton->OnClicked.AddDynamic(this, &UMainWidget::HandleSettingButton);
	}

	if (ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &UMainWidget::HandleExitButton);
	}

}

void UMainWidget::HandleStartButton()
{
	UE_LOG(LogTemp, Log, TEXT("Widget: Start Button Clicked -> Broadcasting OnStartClicked"));
	OnStartClicked.Broadcast();
}

void UMainWidget::HandleSettingButton()
{
	UE_LOG(LogTemp, Log, TEXT("Widget: Setting Button Clicked -> Broadcasting OnSettingClicked"));
	OnSettingClicked.Broadcast();
}

void UMainWidget::HandleExitButton()
{
	UE_LOG(LogTemp, Log, TEXT("Widget: Exit Button Clicked -> Broadcasting OnExitClicked"));
	OnExitClicked.Broadcast();
}
