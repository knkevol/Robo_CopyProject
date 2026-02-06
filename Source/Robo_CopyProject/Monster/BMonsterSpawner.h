// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterSpawnerBase.h"
#include "BMonsterSpawner.generated.h"

UCLASS()
class ROBO_COPYPROJECT_API ABMonsterSpawner : public AMonsterSpawnerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABMonsterSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AActor*> SpawnedBMonsters;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboSpawner")
	TSubclassOf<class APawn> BMonsterClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboSpawner")
	float SpawnDelay = 0.f;

	void SpawnBMonster();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ExecuteSpawn() override;

	UFUNCTION(BlueprintCallable)
	virtual int32 GetAliveMonsterCount() override;

};
