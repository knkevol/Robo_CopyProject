// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboPlayer.h"
#include "RoboPlayerController.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "../Item/PickUpItemBase.h"
#include "../Weapon/WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"

// Sets default values
ARoboPlayer::ARoboPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());

}

void ARoboPlayer::PressG_Implementation(ACharacter* Character)
{
	//인터페이스 PressG 동작 쓸 때 사용
}

void ARoboPlayer::ReleaseG_Implementation()
{
	//인터페이스 ReleaseG 동작 쓸 때 사용
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

	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UIC->BindAction(IA_Equip, ETriggerEvent::Started, this, &ARoboPlayer::Input_PressG);
		UIC->BindAction(IA_Reload, ETriggerEvent::Completed, this, &ARoboPlayer::C2S_Reload);
	}

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
	//APickUpItemBase* PickedItem = Cast<APickUpItemBase>(OtherActor);

	//if (PickedItem)
	//{
	//	switch (PickedItem->ItemType)
	//	{
	//	case EItemType::Weapon:
	//		EquipItem(PickedItem);
	//		break;
	//	case EItemType::Item:
	//		UseItem(PickedItem);
	//		break;
	//	}

	//	PickedItem->SetOwner(this);

	//	if (!PickedItem->bIsItemDestroy)
	//	{
	//		PickedItem->Destroy();
	//	}
	//}
}

void ARoboPlayer::EquipItem(APickUpItemBase* PickedItem)
{
	if (!PickedItem)
		return;

	// 서버에서만 Multicast 호출
	if (HasAuthority())
	{
		Multi_EquipItem(PickedItem);
	}
}

void ARoboPlayer::Multi_EquipItem_Implementation(APickUpItemBase* PickedItem)
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

AActor* ARoboPlayer::FindNearestActor() const
{
	AActor* LocalNearestActor = nullptr;
	float LocalNearestDist = 9999999.0f;

	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors, AActor::StaticClass());

	const FVector SelfLocation = GetActorLocation();

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor)
		{
			continue;
		}

		// Interface_Press 캐스트 (BP의 Cast Interface_Press)
		if (!Actor->GetClass()->ImplementsInterface(UInterface_Press::StaticClass()))
		{
			continue;
		}

		// 거리 계산
		const FVector ActorLocation = Actor->GetActorLocation();
		const float Distance = FVector::Dist(SelfLocation, ActorLocation);

		if (LocalNearestDist > Distance)
		{
			LocalNearestActor = Actor;
			LocalNearestDist = Distance;
		}
	}

	return LocalNearestActor;
}

void ARoboPlayer::Server_PressG_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Server_PressG"));
	PressNearestItem();
}

void ARoboPlayer::Input_PressG()
{

	UE_LOG(LogTemp, Warning, TEXT("Input_PressG"));
	if (!IsLocallyControlled())
	{
		return;
	}

	Server_PressG();
}

void ARoboPlayer::PressNearestItem()
{
	UE_LOG(LogTemp, Warning, TEXT("PressNearestItem()"));
	AActor* NearestActor = FindNearestActor();
	if (!NearestActor)
	{
		return;
	}

	APickUpItemBase* PickedItem = Cast<APickUpItemBase>(NearestActor);
	if (!PickedItem)
	{
		return;
	}

	if (PickedItem->GetClass()->ImplementsInterface(UInterface_Press::StaticClass()))
	{
		IInterface_Press::Execute_PressG(PickedItem, this);
	}
}

void ARoboPlayer::S2A_Reload_Implementation()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		PlayAnimMontage(ChildWeapon->ReloadMontage);
	}
}

void ARoboPlayer::C2S_Reload_Implementation()
{
	S2A_Reload();
}

void ARoboPlayer::ReloadWeapon()
{
	// Bullet = Max
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->Reload();
	}
}
