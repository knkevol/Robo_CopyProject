// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboPlayerController.h"
#include "../Widget/PlayerWidget.h"

void ARoboPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void ARoboPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ARoboPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (PlayerWidgetClass)
		{
			PlayerWidgetObject = CreateWidget<UPlayerWidget>(this, PlayerWidgetClass);
			PlayerWidgetObject->AddToViewport();
		}
	}
}
