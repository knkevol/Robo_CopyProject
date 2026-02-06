// Fill out your copyright notice in the Description page of Project Settings.


#include "ZoneTrigger.h"
#include "Components/BoxComponent.h"
#include "../Player/RoboPlayer.h"
#include "../Monster/MonsterSpawnerBase.h"
#include "../Monster/BMonsterSpawner.h"

// Sets default values
AZoneTrigger::AZoneTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->SetBoxExtent(FVector(300.0f, 50.0f, 50.0f));
	TriggerBox->SetCollisionProfileName(TEXT("SpawnTrigger"));

}

// Called when the game starts or when spawned
void AZoneTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AZoneTrigger::OnBoxOverlap);
	}
	
}

void AZoneTrigger::OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (bIsTriggered || !OtherActor->IsA(ARoboPlayer::StaticClass()))
	{
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor->IsA(ARoboPlayer::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("AZoneTrigger::OnBoxOverlap_OtherActor"));
		for (TObjectPtr<AMonsterSpawnerBase> Spawner : TargetSpawners)
		{
			if (Spawner)
			{
				UE_LOG(LogTemp, Warning, TEXT("AZoneTrigger::OnBoxOverlap_Spawn"));
				Spawner->ExecuteSpawn();
			}
		}
		TriggerBox->OnComponentBeginOverlap.RemoveDynamic(this, &AZoneTrigger::OnBoxOverlap);
	}

	bIsTriggered = true;

	//Destroy();

}

// Called every frame
void AZoneTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AZoneTrigger::IsZoneCleared()
{
	for (auto Spawner : TargetSpawners)
	{
		if (Spawner && Spawner->GetAliveMonsterCount() > 0)
		{
			return false; 
		}
	}
	return true;
}

