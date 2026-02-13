// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginPC.h"
#include "../Widget/LoginWidget.h"

void ALoginPC::BeginPlay()
{
	Super::BeginPlay();

	if (LoginWidgetClass)
	{
		LoginWidgetObject = CreateWidget<ULoginWidget>(this, LoginWidgetClass);
		LoginWidgetObject->AddToViewport();
	}

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}

void ALoginPC::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
