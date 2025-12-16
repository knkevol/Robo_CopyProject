// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboPlayerState.h"
#include "Net/UnrealNetwork.h"

void ARoboPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoboPlayerState, CurHp);
	DOREPLIFETIME(ARoboPlayerState, MaxHp);
}

void ARoboPlayerState::OnRep_CurrentHP()
{
	OnHpChanged.Broadcast(CurHp / MaxHp);
}
