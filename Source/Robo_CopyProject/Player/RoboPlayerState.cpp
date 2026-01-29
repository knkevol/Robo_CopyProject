// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboPlayerState.h"

void ARoboPlayerState::LevelUp()
{
	PlayerLevel++;
	UE_LOG(LogTemp, Log, TEXT("Level Up! Current Level: %d"), PlayerLevel);
}
