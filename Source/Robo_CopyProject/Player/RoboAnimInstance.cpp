// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboAnimInstance.h"
#include "RoboPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void URoboAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	ARoboPlayer* Player = Cast<ARoboPlayer>(TryGetPawnOwner());
	if (Player)
	{
		GroundSpeed = Player->GetCharacterMovement()->Velocity.Size2D();
		Direction = UKismetAnimationLibrary::CalculateDirection(Player->GetCharacterMovement()->Velocity, Player->GetActorRotation());
		AimYaw = Player->GetAimOffset().Yaw;
		AimPitch = Player->GetAimOffset().Pitch;
		WeaponState = Player->WeaponState;

	}

}
