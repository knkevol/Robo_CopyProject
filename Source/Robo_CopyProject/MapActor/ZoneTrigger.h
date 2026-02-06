// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZoneTrigger.generated.h"

UCLASS()
class ROBO_COPYPROJECT_API AZoneTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZoneTrigger();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "RoboTrigger")
	bool IsZoneCleared();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="RoboTrigger")
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboTrigger")
	TArray<TObjectPtr<class AMonsterSpawnerBase>> TargetSpawners;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bIsTriggered = false;

};
