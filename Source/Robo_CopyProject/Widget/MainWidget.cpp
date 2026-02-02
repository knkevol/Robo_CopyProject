// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"

void UMainWidget::OnStartButtonClicked()
{
	const UWorld* World = GetWorld();
	check(World);
	UGameplayStatics::OpenLevel(World, TEXT("Stage"));
	
}