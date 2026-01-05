// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboPlayer.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARoboPlayer::ARoboPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoboPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoboPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARoboPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARoboPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoboPlayer, CurHp);
	DOREPLIFETIME(ARoboPlayer, MaxHp);
}

void ARoboPlayer::OnRep_CurrentHP()
{
	OnHpChanged.Broadcast(CurHp / MaxHp);
}

