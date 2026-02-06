// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboBossMonster.h"
#include "Net/UnrealNetwork.h" //Replicated

// Sets default values
ARoboBossMonster::ARoboBossMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionProfileName(TEXT("MonsterMesh"));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ARoboBossMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoboBossMonster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoboBossMonster, CurrentHP);
	DOREPLIFETIME(ARoboBossMonster, MaxHP);
	DOREPLIFETIME(ARoboBossMonster, CurrentState);
}

void ARoboBossMonster::SetState(EBMonsterState NewState)
{
	if (HasAuthority())
	{
		CurrentState = NewState;
	}
}

void ARoboBossMonster::OnRep_BMonsterCurrentHP()
{
}

// Called every frame
void ARoboBossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARoboBossMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

