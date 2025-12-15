// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboMonster.h"

// Sets default values
ARoboMonster::ARoboMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoboMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoboMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARoboMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

