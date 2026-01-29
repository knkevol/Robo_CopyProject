// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboPlayerController.h"
#include "../Widget/PlayerWidget.h"
#include "../Widget/LevelUpWidget.h"
#include "RoboPlayer.h"

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

		if (LevelUpWidgetClass)
		{
			LevelUpWidgetObject = CreateWidget<ULevelUpWidget>(this, LevelUpWidgetClass);
		}
	}


	if (APawn* RoboPawn = GetPawn())
	{
		if (ARoboPlayer* RoboPlayer = Cast<ARoboPlayer>(RoboPawn))
		{
			RoboPlayer->SetPlayerWidget(PlayerWidgetObject);
			RoboPlayer->SetLevelUpWidget(LevelUpWidgetObject);
		}
	}
}

void ARoboPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (ARoboPlayer* RoboPlayer = Cast<ARoboPlayer>(aPawn))
	{
		RoboPlayer->SetPlayerWidget(PlayerWidgetObject);
		RoboPlayer->SetLevelUpWidget(LevelUpWidgetObject);
	}
}

void ARoboPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

