// Fill out your copyright notice in the Description page of Project Settings.


#include "BMonsterSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"

// Sets default values
ABMonsterSpawner::ABMonsterSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void ABMonsterSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABMonsterSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABMonsterSpawner::ExecuteSpawn()
{
    UE_LOG(LogTemp, Warning, TEXT("ABMonsterSpawner::ExecuteSpawn()"));
    SpawnBMonster();
}

void ABMonsterSpawner::SpawnBMonster()
{
    if (!BMonsterClass)
    {
        return;
    }

    FActorSpawnParameters Params;
    Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    APawn* Monster = GetWorld()->SpawnActor<APawn>(
        BMonsterClass,
        GetActorTransform(),
        Params
    );

    if (Monster)
    {
        SpawnedBMonsters.Add(Monster);
        UE_LOG(LogTemp, Warning, TEXT("BMonster Spawned"));
    }
}

int32 ABMonsterSpawner::GetAliveMonsterCount()
{
    int32 Count = 0;
    for (AActor* BMonster : SpawnedBMonsters)
    {
        if (IsValid(BMonster))
        {
            Count++;
        }
    }
    return Count;
}

