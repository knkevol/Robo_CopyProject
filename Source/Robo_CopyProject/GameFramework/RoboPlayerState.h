// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "RoboPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedHP, const float, Percent);

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API ARoboPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData", ReplicatedUsing = "OnRep_CurrentHP")
	float CurHp = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerData", Replicated)
	float MaxHp = 100;
	// 복제 통지 함수
	UFUNCTION()
	void OnRep_CurrentHP();

	UPROPERTY(BlueprintAssignable)
	FOnChangedHP OnHpChanged;

	
};
