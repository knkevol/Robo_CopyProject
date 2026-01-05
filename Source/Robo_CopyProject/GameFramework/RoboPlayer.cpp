// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboPlayer.h"
#include "RoboPlayerController.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "../Item/PickUpItemBase.h"
#include "../Weapon/WeaponBase.h"

#include "Net/UnrealNetwork.h"

// Sets default values
ARoboPlayer::ARoboPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());

}

// Called when the game starts or when spawned
void ARoboPlayer::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ARoboPlayer::ProcessBeginOverlap);
	
}

// Called every frame
void ARoboPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARoboPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ARoboPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoboPlayer, CurHp);
	DOREPLIFETIME(ARoboPlayer, MaxHp);
}

void ARoboPlayer::OnRep_CurrentHP()
{
	OnHpChanged.Broadcast(CurHp / MaxHp);
}

void ARoboPlayer::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ARoboPlayerController* RPC = Cast<ARoboPlayerController>(GetController());
	if (!RPC || !RPC->HasAuthority())
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("ProcessBeginOverlap"));
	APickUpItemBase* PickedItem = Cast<APickUpItemBase>(OtherActor);

	if (PickedItem)
	{
		switch (PickedItem->ItemType)
		{
		case EItemType::Weapon:
			EquipItem(PickedItem);
			break;
		case EItemType::Item:
			UseItem(PickedItem);
			break;
		}

		PickedItem->SetOwner(this);

		if (!PickedItem->bIsItemDestroy)
		{
			PickedItem->Destroy();
		}
	}
}

void ARoboPlayer::EquipItem(APickUpItemBase* PickedItem)
{
	Weapon->SetChildActorClass(PickedItem->ItemTemplate);
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (!ChildWeapon)
	{
		return;
	}

	if (ChildWeapon)
	{
		if (ChildWeapon->Name.Compare(TEXT("Pistol")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Pistol;
			ChildWeapon->SetOwner(this);
		}
		else if (ChildWeapon->Name.Compare(TEXT("Rifle")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Rifle;
			ChildWeapon->SetOwner(this);
		}
		else if (ChildWeapon->Name.Compare(TEXT("ScifiKnife")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::ScifiKnife;
			ChildWeapon->SetOwner(this);
		}
		else if (ChildWeapon->Name.Compare(TEXT("ScifiWeapon01")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::ScifiWeapon01;
			ChildWeapon->SetOwner(this);
		}
		else if (ChildWeapon->Name.Compare(TEXT("ScifiWeapon02")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::ScifiWeapon02;
			ChildWeapon->SetOwner(this);
		}
		UE_LOG(LogTemp, Warning, TEXT("Weapon : %s"), *ChildWeapon->GetOuter()->GetName());
	}
}

void ARoboPlayer::UseItem(APickUpItemBase* PickedItem)
{
}

