// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/ItemBase.h"
#include "WeaponBase.generated.h"

class UStaticMeshComponent;
class AProjectileBase;
class UAnimMontage;

/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	AWeaponBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoboWeaponComponent")
	TObjectPtr<UStaticMeshComponent> Mesh;

	// Weapon Info
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	TSubclassOf<AProjectileBase> ProjectileTemplate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	FName SocketName = TEXT("HandGrip_R");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	FName Name;
};
