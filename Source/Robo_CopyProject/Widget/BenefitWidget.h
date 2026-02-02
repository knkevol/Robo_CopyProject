// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BenefitWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBenefitClicked);

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API UBenefitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable, Category = "RoboWidget")
    FOnBenefitClicked OnBenefitClicked;

protected:
    // 마우스가 들어왔을 때 (Hover Start)
    virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    // 마우스가 나갔을 때 (Hover End)
    virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

    // 마우스 클릭 시
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

    //// 블루프린트에서 애니메이션을 제어하기 위한 변수 (선택 사항)
    //UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidgetAnim), Transient)
    //TObjectPtr<class UWidgetAnimation> HoverAnimation;

	
};
