// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;
class UStaticMeshComponent;
class UBoxComponent;

UCLASS()
class ROBO_COPYPROJECT_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<UProjectileMovementComponent> Movement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	float Damage = 10.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TSubclassOf<UDamageType> HitDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	FHitResult HitResult;

	UFUNCTION()
	void ProcessBeginOverlap(AActor* OverlapedActor, AActor* OtherActor);

	// ----------Deligate
	UFUNCTION()
	void ProcessComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


};
