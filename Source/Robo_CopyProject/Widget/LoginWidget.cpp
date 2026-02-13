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
}

void ULoginWidget::Connect()
{
	SaveData();
	//UGameplayStatics::OpenLevel(GetWorld(), FName(ServerText->GetText().ToString()), true, TEXT("MainWorld"));
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