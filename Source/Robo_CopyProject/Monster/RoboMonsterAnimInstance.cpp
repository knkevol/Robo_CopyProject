// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboMonsterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void URoboMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ARoboMonster* RMonster = Cast<ARoboMonster>(TryGetPawnOwner());
	if (RMonster)
	{
		Speed = RMonster->GetCharacterMovement()->Velocity.Size2D();
		CurrentState = RMonster->CurrentState;
	}
}
