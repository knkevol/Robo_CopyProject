// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "../Player/RoboPlayer.h"
#include "../Player/RoboPlayerController.h"

void UPlayerStatWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	APlayerController* PC = Cast<APlayerController>(GetOwningPlayer());
	if (PC)
	{
		ARoboPlayer* Pawn = Cast<ARoboPlayer>(PC->GetPawn());
		if (Pawn)
		{
			Pawn->OnHpChanged.AddDynamic(this, &UPlayerStatWidget::ProcessHPBar);
			Pawn->OnRep_CurrentHP();
		}
	}
}

void UPlayerStatWidget::ProcessHPBar(float InPercent)
{
	if (PlayerHPBar)
	{
		PlayerHPBar->SetPercent(InPercent);
	}

	if (CurHPText)
	{
		int32 HPPercent = FMath::FloorToInt(InPercent * 200.0f);
		CurHPText->SetText(FText::FromString(FString::Printf(TEXT("%d"), HPPercent)));
	}
}

void UPlayerStatWidget::ProcessXPBar(float InPercent)
{
	UE_LOG(LogTemp, Log, TEXT("UPlayerStatWidget::ProcessXPBar"));
	if (PlayerXPBar)
	{
		UE_LOG(LogTemp, Log, TEXT("UPlayerStatWidget::ProcessXPBar_PlayerXPBar"));
		PlayerXPBar->SetPercent(InPercent);
	}

	if (PlayerLevelText)
	{

	}

}
