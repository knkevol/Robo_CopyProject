// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboBMonsterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

void URoboBMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ARoboBossMonster* BMonster = Cast<ARoboBossMonster>(TryGetPawnOwner());
	if (BMonster)
	{
		Speed = BMonster->GetCharacterMovement()->Velocity.Size2D();
		CurrentState = BMonster->GetCurrentState();
	}
}
