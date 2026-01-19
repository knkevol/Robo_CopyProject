// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/Interface_Press.h"
#include "ItemWidgetActor.generated.h"

UCLASS()
class ROBO_COPYPROJECT_API AItemWidgetActor : public AActor, public IInterface_Press
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemWidgetActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
