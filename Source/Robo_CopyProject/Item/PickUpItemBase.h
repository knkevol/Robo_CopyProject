// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/Interface_Press.h"
#include "PickUpItemBase.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Item = 0 UMETA(DisplayName = "Item"),
	Weapon = 10 UMETA(DisplayName = "Weapon")
};

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class ROBO_COPYPROJECT_API APickUpItemBase : public AActor, public IInterface_Press
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "RoboData")
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "RoboData")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	EItemType ItemType = EItemType::Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TSubclassOf<class AItemBase> ItemTemplate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	uint8 bIsItemDestroy : 1 = false;

	//-------------Interface_Press
	virtual void PressE_Implementation(ACharacter* Character) override;

};
