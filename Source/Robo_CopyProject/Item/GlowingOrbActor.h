// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlowingOrbActor.generated.h"

class USceneComponent;
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
	TObjectPtr<UStaticMeshComponent> OrbMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<UNiagaraComponent> OrbFX;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData", ReplicatedUsing = "OnRep_OrbActive")
	uint8 bOrbActive : 1 = true;

	// 오브 색상
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData", ReplicatedUsing = "OnRep_OrbColor")
	FLinearColor OrbColor;

	UFUNCTION()
	void OnRep_OrbActive();

	UFUNCTION()
	void OnRep_OrbColor();

	void SetOrbActive(bool bNewActive);
	void SetOrbColor(const FLinearColor& NewColor);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
