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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<class UStaticMeshComponent> DoorWay;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<class UStaticMeshComponent> Left;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TObjectPtr<class UStaticMeshComponent> Right;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	FName InputKey;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	float TargetYaw = 90.f; //Default

	FTimerHandle MoveTimerHandle;
	FRotator LeftInitRot;
	FRotator RightInitRot;
	float MoveAlpha = 0.f;


	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	/** 겹침이 끝날 때 호출될 함수 */
	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData", ReplicatedUsing = "OnRep_IsDoorOpen")
	uint8 bIsOpen : 1 = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	FVector ClosedLocation;

	virtual void InteractDoor(ACharacter* Interactor) override;

	UFUNCTION()
	void StartOpenMove();

	UFUNCTION()
	void UpdateDoorMove();

	UFUNCTION()
	void OnRep_IsDoorOpen();

};
