// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "../Widget/MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AMainGameModeBase::AMainGameModeBase()
{
	//PlayerController, World X
}

void AMainGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	UE_LOG(LogTemp, Warning, TEXT("AMainGameModeBase::BeginPlay() PC : %s"), *PC->GetName());
	UE_LOG(LogTemp, Warning, TEXT("AMainGameModeBase::BeginPlay() MainWidgetClass : %s"), *MainWidgetClass->GetName());
	if (PC && MainWidgetClass)
	{
		MainWidgetObject = CreateWidget<UMainWidget>(PC, MainWidgetClass);

		if (MainWidgetObject)
		{
			MainWidgetObject->AddToViewport();
			MainWidgetObject->OnStartClicked.AddDynamic(this, &AMainGameModeBase::OnStartButtonClicked);
			MainWidgetObject->OnSettingClicked.AddDynamic(this, &AMainGameModeBase::OnSettingButtonClicked);
			MainWidgetObject->OnExitClicked.AddDynamic(this, &AMainGameModeBase::OnExitButtonClicked);

			PC->SetShowMouseCursor(true);
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MainWidgetObject->TakeWidget());
			PC->SetInputMode(InputMode);
		}
	}
}

void AMainGameModeBase::OnStartButtonClicked()
{

	UE_LOG(LogTemp, Warning, TEXT("Game Start Button Clicked!"));

	// 마우스 커서 숨기기 및 게임 모드로 전환
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->SetShowMouseCursor(false);
		PC->SetInputMode(FInputModeGameOnly());
	}

	const UWorld* World = GetWorld();
	check(World);
	UGameplayStatics::OpenLevel(World, TEXT("Stage"));

}

void AMainGameModeBase::OnSettingButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Setting Button Clicked!"));
}

void AMainGameModeBase::OnExitButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Exit Button Clicked!"));
	UE_LOG(LogTemp, Warning, TEXT("GameMode: Received Exit Signal"));
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
