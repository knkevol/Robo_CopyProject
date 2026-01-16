// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RoboPlayer.h"
#include "RoboAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API URoboAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	// ----------------Player State
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboPlayer")
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboPlayer")
	EWeaponState WeaponState = EWeaponState::Unarmed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboPlayer")
	float Direction;



	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboPlayer")
	float AimYaw;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboPlayer")
	float AimPitch;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboPlayer")
	FRotator AimRotation;

	
	
};
