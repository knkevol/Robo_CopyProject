// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RoboPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ARoboPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoboWidget")
	TSubclassOf<class UPlayerWidget> PlayerWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoboWidget")
	TObjectPtr<class UPlayerWidget> PlayerWidgetObject;

	//----------RPC--------------
	UFUNCTION(Server, Reliable)
	void Server_RequestNextStage();
	void Server_RequestNextStage_Implementation();
	//---------------------------

	// 입력 모드 복구용 함수
	void SetGameInputMode(uint8 InMode);
	
};
