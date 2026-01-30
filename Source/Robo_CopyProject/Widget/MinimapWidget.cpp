// Fill out your copyright notice in the Description page of Project Settings.


#include "MinimapWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UMinimapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UMinimapWidget::SetMinimapImage(UMaterialInstanceDynamic* InMI)
{
    if (!PMinimapImage)
    {
        UE_LOG(LogTemp, Error, TEXT("MinimapWidget: MinimapImage(Image Widget) is NULL!"));
        return;
    }

    if (!InMI)
    {
        UE_LOG(LogTemp, Error, TEXT("MinimapWidget: InMI(Material) is NULL!"));
        return;
    }

    // 함수가 정상 호출됨을 알림
    UE_LOG(LogTemp, Warning, TEXT("MinimapWidget: Setting Material %s to Image Widget"), *InMI->GetName());
    PMinimapImage->SetBrushFromMaterial(InMI);
}
