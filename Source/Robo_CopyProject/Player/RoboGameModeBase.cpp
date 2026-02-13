// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboGameModeBase.h"
#include "RoboPlayerController.h"

#include "../Widget/StageNameWidget.h"
#include "../Widget/ClearWidget.h"

#include "Kismet/GameplayStatics.h"

ARoboGameModeBase::ARoboGameModeBase()
{
}

void ARoboGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (StageNameWidgetClass)
	{
		StageNameWidgetObject = CreateWidget<UStageNameWidget>(GetWorld(), StageNameWidgetClass);
		if (StageNameWidgetObject)
		{
			StageNameWidgetObject->AddToViewport();

			FString LevelName = TEXT("[ Crypt ]");
			StageNameWidgetObject->StageNameTyping(LevelName, 0.2f);
		}
	}
}

void ARoboGameModeBase::NextStage()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		if (ARoboPlayerController* PC = Cast<ARoboPlayerController>(It->Get()))
		{
			PC->SetGameInputMode(false);
		}
	}
	GetWorld()->ServerTravel(TEXT("Stage2"));
}
