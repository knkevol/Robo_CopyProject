// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTopWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "../Monster/RoboBossMonster.h"
#include "Kismet/GameplayStatics.h"

void UPlayerTopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UPlayerTopWidget::ProcessBossHPBar(float InPercent)
{
	if (BossHPBar)
	{
		BossHPBar->SetPercent(InPercent);
	}	
}

void UPlayerTopWidget::UpdateBossInfo(ARoboBossMonster* TargetBoss)
{
	if (BossNameText)
	{
		BossNameText->SetText(FText::FromString(TargetBoss->BossName.ToString()));
	}

	if (TargetBoss)
	{
		TargetBoss->OnBossHpChanged.RemoveDynamic(this, &UPlayerTopWidget::ProcessBossHPBar);
		TargetBoss->OnBossHpChanged.AddDynamic(this, &UPlayerTopWidget::ProcessBossHPBar);
		ProcessBossHPBar(TargetBoss->GetCurrentHP() / TargetBoss->GetMaxHP());
	}
}
