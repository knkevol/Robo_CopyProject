// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "../GameFramework/RoboPlayer.h"

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
	if (PlayerHP)
	{

		PlayerHP->SetPercent(InPercent);

	}
}
