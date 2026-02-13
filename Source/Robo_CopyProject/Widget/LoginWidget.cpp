// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"

#include "Components/EditableTextBox.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"

#include "../Login/LoginGameInstanceSubsystem.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ServerText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("ServerText")));

	ConnectButton->OnClicked.AddDynamic(this, &ULoginWidget::Connect);

	UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
	if (GI)
	{
		ULoginGameInstanceSubsystem* Subsystem = GI->GetSubsystem<ULoginGameInstanceSubsystem>();
		Subsystem->OnLoginResult.AddDynamic(this, &ULoginWidget::LoginResult);
	}
}

void ULoginWidget::Connect()
{
	SaveData();
}

void ULoginWidget::SaveData()
{
	UGameInstance* GI = UGameplayStatics::GetGameInstance(GetWorld());
	if (GI)
	{
		ULoginGameInstanceSubsystem* MySubsystem = GI->GetSubsystem<ULoginGameInstanceSubsystem>();
		MySubsystem->ID = IDText->GetText().ToString();
		MySubsystem->Password = PasswordText->GetText().ToString();

		MySubsystem->Login();
	}
}

void ULoginWidget::LoginResult(bool InResult)
{
	if (InResult)
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainWorld"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Failed"));
	}
}
