// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.h"
#include "DoorActor.generated.h"

UCLASS()
class ROBO_COPYPROJECT_API ADoorActor : public AInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Robo", ReplicatedUsing = "OnRep_IsDoorOpen")
	uint8 bIsOpen : 1;

	virtual void InteractDoor(ACharacter* Interactor) override;

	UFUNCTION()
	void OnRep_IsDoorOpen();

};
