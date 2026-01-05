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
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "RoboComponent")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TSubclassOf<AProjectileBase> ProjectileTemplate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	FName SocketName = TEXT("Gun_LOS");
};
