// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Character.h"
#include "../Player/RoboPlayer.h"
#include "Camera/CameraComponent.h"
#include "ProjectileBase.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	SetReplicates(true);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (ARoboPlayer* Player = GetOwningPlayer())
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Owner %s"), *GetOwningPlayer()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon Owner XXXXXXX"));
	}
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ARoboPlayer* AWeaponBase::GetOwningPlayer() const
{
	if (AActor* Parent = GetParentActor())
	{
		return Cast<ARoboPlayer>(Parent);
	}
	return nullptr;
}

void AWeaponBase::Reload()
{
	CurBullet = MaxBullet;
	UE_LOG(LogTemp, Warning, TEXT("Reload %d"), CurBullet);
}

void AWeaponBase::Fire()
{
	float CurrentTimeofShoot = GetWorld()->TimeSeconds - TimeofLastShoot;

	if (CurrentTimeofShoot < RefireRate)
	{
		return;
	}

	//if (bFullAuto)
	//{
	//	GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AWeaponBase::Fire, RefireRate, false);
	//}
	GetWorld()->GetTimerManager().SetTimer(RefireTimer, this, &AWeaponBase::Fire, RefireRate, false);

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (!Character)
	{
		return;
	}

	//Calculate
	FVector SpawnLocation;
	FVector TargetLocation;
	FRotator AimRotation;
	FHitResult HitResult;

	bool bResult = CalculateShootData(SpawnLocation, TargetLocation, AimRotation);
	if (!bResult)
	{
		return;
	}

	FireProjectile(FTransform(AimRotation, SpawnLocation, FVector::OneVector), HitResult);

	//Recoil
	Character->AddControllerPitchInput(-0.05f);


	CurBullet--;
	UE_LOG(LogTemp, Warning, TEXT("Fire %d"), CurBullet);


	TimeofLastShoot = GetWorld()->TimeSeconds;
}

void AWeaponBase::StopFire()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Fire"));
	GetWorld()->GetTimerManager().ClearTimer(RefireTimer);
}

void AWeaponBase::FireProjectile(FTransform SpawnTransform, FHitResult InHitResult)
{
	AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileTemplate, SpawnTransform);
	Projectile->HitResult = InHitResult;
	Projectile->SetOwner(this);
}

bool AWeaponBase::CalculateShootData(FVector& OutSpawnLocation, FVector& OutTargetLocation, FRotator& OutAimRotation)
{

	ARoboPlayer* RoboPlayer = Cast<ARoboPlayer>(GetOwner());

	if (!RoboPlayer)
	{
		return false;
	}

	FHitResult OutHit;

	FVector ViewLocation;
	FRotator ViewRotation;

	if (AController* Controller = RoboPlayer->GetController())
	{
		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		// Controller가 없을 때 fallback
		ViewLocation = RoboPlayer->GetPawnViewLocation();
		ViewRotation = RoboPlayer->GetViewRotation();
	}

	const FVector Start = ViewLocation;
	const FVector End = Start + (ViewRotation.Vector() * 10000.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(RoboPlayer);

	GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, QueryParams);

	//Calculate
	OutSpawnLocation = Mesh->GetSocketLocation(TEXT("Muzzle"));
	OutTargetLocation = OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;

	OutAimRotation = UKismetMathLibrary::FindLookAtRotation(OutSpawnLocation, OutTargetLocation + (UKismetMathLibrary::RandomUnitVector() * 0.3f));

	return true;
}