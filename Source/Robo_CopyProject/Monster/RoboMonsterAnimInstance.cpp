// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboMonsterAnimInstance.h"

void URoboMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ARoboMonster* RMonster = Cast<ARoboMonster>(TryGetPawnOwner());
	if (RMonster)
	{
		CurrentState = RMonster->GetCurrentState();
	}
}
