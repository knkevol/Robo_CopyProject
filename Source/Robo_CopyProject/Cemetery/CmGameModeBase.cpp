// Fill out your copyright notice in the Description page of Project Settings.


#include "CmGameModeBase.h"
#include "../Widget/StageNameWidget.h"

ACmGameModeBase::ACmGameModeBase()
{
}

void ACmGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Super::BeginPlay();

	if (StageNameWidgetClass)
	{
		StageNameWidgetObject = CreateWidget<UStageNameWidget>(GetWorld(), StageNameWidgetClass);
		if (StageNameWidgetObject)
		{
			StageNameWidgetObject->AddToViewport();

			FString LevelName = TEXT("Cemetery");
			StageNameWidgetObject->StageNameTyping(LevelName, 0.3f);
		}
	}
}
