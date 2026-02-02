// Fill out your copyright notice in the Description page of Project Settings.


#include "StageNameWidget.h"
#include "Components/TextBlock.h"

void UStageNameWidget::HideStageNameWidget()
{
	RemoveFromParent();
}

void UStageNameWidget::StageNameTyping(FString TargetText, float TypingSpeed)
{
    FullText = TargetText;
    CurrentCharIndex = 0;

    if (StageNameText)
    {
        StageNameText->SetText(FText::GetEmpty());
    }

    // 이전 타이머가 있다면 초기화
    GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);

    // 타이머 시작 (TypingSpeed 간격으로 OnTypingTick 호출)
    GetWorld()->GetTimerManager().SetTimer(TypingTimerHandle, this, &UStageNameWidget::OnTypingTick, TypingSpeed, true);
}

void UStageNameWidget::OnTypingTick()
{
    if (CurrentCharIndex < FullText.Len())
    {
        CurrentCharIndex++;
        // 문장에서 현재 인덱스까지 잘라서 출력
        FString SubString = FullText.Left(CurrentCharIndex);
        StageNameText->SetText(FText::FromString(SubString));
    }
    else
    {
        // 타이핑 종료 시 타이머 해제
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);

        FTimerHandle HideHandle;
        GetWorld()->GetTimerManager().SetTimer(HideHandle, this, &UStageNameWidget::HideStageNameWidget, 3.0f, false);
    }
}
