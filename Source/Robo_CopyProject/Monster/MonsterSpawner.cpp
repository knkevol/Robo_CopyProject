// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

// Sets default values
AMonsterSpawner::AMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterSpawner::ExecuteSpawn()
{
    UE_LOG(LogTemp, Warning, TEXT("AMonsterSpawner::ExecuteSpawn()"));
    SpawnMonster();
}

void AMonsterSpawner::SpawnMonster()
{
    if (!MonsterClass)
    {
        return;
    }

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    APawn* Monster = GetWorld()->SpawnActor<APawn>(
        MonsterClass,
        GetActorTransform(),
        Params
    );

    if (Monster)
    {
        SpawnedMonsters.Add(Monster);
        UE_LOG(LogTemp, Warning, TEXT("Monster Spawned"));
    }
}

int32 AMonsterSpawner::GetAliveMonsterCount()
{
    int32 Count = 0;
    for (AActor* Monster : SpawnedMonsters)
    {
        if (IsValid(Monster))
        {
            Count++;
        }
    }
    return Count;
}

