// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboPlayerController.h"
#include "RoboPlayer.h"
#include "RoboGameModeBase.h"

#include "../Widget/PlayerWidget.h"


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


	if (APawn* RoboPawn = GetPawn())
	{
		if (ARoboPlayer* RoboPlayer = Cast<ARoboPlayer>(RoboPawn))
		{
			RoboPlayer->SetPlayerWidget(PlayerWidgetObject);
		}
	}
}

void ARoboPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	if (ARoboPlayer* RoboPlayer = Cast<ARoboPlayer>(aPawn))
	{
		RoboPlayer->SetPlayerWidget(PlayerWidgetObject);
	}
}

void ARoboPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ARoboPlayerController::Server_RequestNextStage_Implementation()
{
	if (ARoboGameModeBase* GM = Cast<ARoboGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->NextStage();
	}
}

void ARoboPlayerController::SetGameInputMode(uint8 InMode)
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = InMode;
}

