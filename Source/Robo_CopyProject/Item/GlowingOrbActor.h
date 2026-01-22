// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlowingOrbActor.generated.h"

class USceneComponent;
class USphereComponent;
class UStaticMeshComponent;
class UNiagaraComponent;

UCLASS()
class ROBO_COPYPROJECT_API AGlowingOrbActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlowingOrbActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<USphereComponent> OrbCollision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<UStaticMeshComponent> OrbMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<UNiagaraComponent> OrbFX;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData", ReplicatedUsing = "OnRep_OrbExist")
	uint8 bOrbExist : 1 = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	float HealAmount = 20.f;


	UFUNCTION()
	void OnRep_OrbExist();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
