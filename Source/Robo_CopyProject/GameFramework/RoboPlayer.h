// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "RoboPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedHP, const float, Percent);

UCLASS()
class ROBO_COPYPROJECT_API ARoboPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoboPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_CurrentHP();
	// -----Player State-----
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerStat", ReplicatedUsing = "OnRep_CurrentHP")
	float CurHp = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PlayerStat", Replicated)
	float MaxHp = 100;

	// 복제 통지 함수
	UPROPERTY(BlueprintAssignable)
	FOnChangedHP OnHpChanged;

};
