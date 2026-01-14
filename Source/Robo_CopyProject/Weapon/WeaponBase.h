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

	// ---------------Weapon Info
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	TSubclassOf<AProjectileBase> ProjectileTemplate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	FName SocketName = TEXT("HandGrip_R");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	int32 MaxBullet = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	int32 CurBullet = 100;

	// ---------------Weapon Action
	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void FireProjectile(FTransform SpawnTransform, FHitResult InHitResult);

	// ---------------Montages
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "RoboMontage")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboMontage")
	TObjectPtr<UAnimMontage> ReloadMontage;

};
