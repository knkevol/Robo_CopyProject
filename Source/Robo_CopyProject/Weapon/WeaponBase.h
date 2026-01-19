// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Item/ItemBase.h"
#include "../Interface/Interface_Press.h"
#include "WeaponBase.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnBulletChanged, int32, int32);

class USkeletalMeshComponent;
class AProjectileBase;
class UAnimMontage;
class ARoboPlayer;


/**
 * 
 */
UCLASS()
class ROBO_COPYPROJECT_API AWeaponBase : public AItemBase, public IInterface_Press
{
	GENERATED_BODY()

public:
	AWeaponBase();

	FOnBulletChanged OnBulletChanged;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoboWeaponComponent")
	TObjectPtr<USkeletalMeshComponent> Mesh;

	// ---------------Weapon Info
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	TSubclassOf<AProjectileBase> ProjectileTemplate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	FName SocketName = TEXT("HandGrip_R");

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	int32 MaxBullet = 10;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	int32 CurBullet = 10;


	ARoboPlayer* GetOwningPlayer() const;

	// ---------------Weapon Action
	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void Fire();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData", meta = (ClampMin = 0.1f, ClampMax = 2.0f, Unit = "s"))
	float RefireRate = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboWeaponData")
	float TimeofLastShoot = 0.0f;
	UFUNCTION(BlueprintCallable)
	void FireProjectile(FTransform SpawnTransform, FHitResult InHitResult);
	FTimerHandle RefireTimer;
	bool CalculateShootData(FVector& OutSpawnLocation, FVector& OutTargetLocation, FRotator& OutAimRotation);

	UFUNCTION(BlueprintCallable)
	void StopFire();

	// ---------------Montages
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category = "RoboMontage")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboMontage")
	TObjectPtr<UAnimMontage> ReloadMontage;

};
