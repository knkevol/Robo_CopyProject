// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include "ProjectileWidget.h"
#include "PlayerTopWidget.h"

#include "Kismet/GameplayStatics.h"
#include "../Monster/RoboBossMonster.h"

void UPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (PlayerTopWidget)
	{
		SetTopWidgetVisibility(false);
	}

	if (ProjectileWidget)
	{
		ProjectileWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UPlayerWidget::UpdateBullet(int32 Cur, int32 Max)
{
	if (ProjectileWidget)
	{
		ProjectileWidget->SetWidgetBullet(Cur, Max);
	}
}

void UPlayerWidget::SetTopWidgetVisibility(bool bIsVisible)
{
	if (PlayerTopWidget)
	{
		if (bIsVisible)
		{
			// 위젯을 켜기 직전에 보스를 다시 찾음
			AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ARoboBossMonster::StaticClass());
			ARoboBossMonster* Boss = Cast<ARoboBossMonster>(FoundActor);

			if (Boss)
			{
				// 이름 설정 및 델리게이트 바인딩
				PlayerTopWidget->UpdateBossInfo(Boss);
				PlayerTopWidget->SetVisibility(ESlateVisibility::Visible);
			}
			UE_LOG(LogTemp, Warning, TEXT("SetTopWidgetVisibility_Boss : %s"), Boss ? *Boss->GetName() : TEXT("None BOss"));
		}
		else
		{
			PlayerTopWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}
