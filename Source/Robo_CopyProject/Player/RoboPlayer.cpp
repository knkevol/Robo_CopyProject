// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboPlayer.h"
#include "RoboPlayerController.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "../Item/PickUpItemBase.h"
#include "../Weapon/WeaponBase.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "../Widget/PlayerWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../MapActor/InteractableActor.h"
#include "../MapActor/DoorActor.h"


// Sets default values
ARoboPlayer::ARoboPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Weapon = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Capsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Capsule->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSource"));

	SetGenericTeamId(1);
}

// Called when the game starts or when spawned
void ARoboPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (GetMesh())
	{
		GetMesh()->SetOwnerNoSee(false);
		GetMesh()->SetOnlyOwnerSee(false);
		GetMesh()->SetVisibility(true, true);
	}

	OnActorBeginOverlap.AddDynamic(this, &ARoboPlayer::ProcessBeginOverlap);

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->OnComponentBeginOverlap.AddDynamic(this,&ARoboPlayer::OnBeginOverlap);
	Capsule->OnComponentEndOverlap.AddDynamic(this,	&ARoboPlayer::OnEndOverlap);
	
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
	UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent"));
	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIC"));
		UIC->BindAction(IA_Equip, ETriggerEvent::Started, this, &ARoboPlayer::Input_PressE);
		UIC->BindAction(IA_DoorOpen, ETriggerEvent::Started, this, &ARoboPlayer::Input_PressF);
		UIC->BindAction(IA_Reload, ETriggerEvent::Completed, this, &ARoboPlayer::Server_Reload);

		UIC->BindAction(IA_Fire, ETriggerEvent::Started, this, &ARoboPlayer::StartFire);
		UIC->BindAction(IA_Fire, ETriggerEvent::Completed, this, &ARoboPlayer::StopFire);
	}

}

void ARoboPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoboPlayer, CurHp);
	DOREPLIFETIME(ARoboPlayer, MaxHp);
}

void ARoboPlayer::SetPlayerWidget(UPlayerWidget* InWidget)
{
	PlayerWidgetObject = InWidget;
}

void ARoboPlayer::OnRep_CurrentHP()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_CurrentHP"));
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

}

float ARoboPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("ARoboPlayer::TakeDamage"));
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurHp <= 0.0f)
	{
		return DamageAmount;
	}

	CurHp -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Player TakeDamage : %f"), CurHp);
	OnRep_CurrentHP();
	Multi_PlayerSpawnHitEffect(GetActorLocation(), GetActorRotation());

	if (CurHp <= 0.0f)
	{
		Multi_PlayerDie();
	}

	return 0.0f;
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

void ARoboPlayer::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult)
{

	if (AInteractableActor* Interactable = Cast<AInteractableActor>(OtherActor))
	{
		FocusedActor = Interactable;
		UE_LOG(LogTemp, Warning, TEXT("FocusedActor SET: %s"), *OtherActor->GetName());
	}
}

void ARoboPlayer::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == FocusedActor)
	{
		FocusedActor = nullptr;
	}
}

AActor* ARoboPlayer::FindNearestActor() const
{
	AActor* NearestActor = nullptr;
	float NearestDistSq = FLT_MAX;

	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);

	const FVector SelfLocation = GetActorLocation();

	for (AActor* Actor : OverlappingActors)
	{
		if (!Actor)
			continue;

		// 🔥 인터페이스 필터
		if (!Actor->GetClass()->ImplementsInterface(UInterface_Press::StaticClass()))
			continue;

		const float DistSq = FVector::DistSquared(SelfLocation, Actor->GetActorLocation());
		if (DistSq < NearestDistSq)
		{
			NearestActor = Actor;
			NearestDistSq = DistSq;
		}
	}

	return NearestActor;
}

void ARoboPlayer::Server_EquipWeapon_Implementation(TSubclassOf<AWeaponBase> WeaponClass)
{
	CurrentWeaponClass = WeaponClass;
	SetWeaponChildActor(WeaponClass);
}

void ARoboPlayer::Input_PressE()
{

	UE_LOG(LogTemp, Warning, TEXT("Input_PressE"));
	if (!IsLocallyControlled())
		return;

	AActor* TargetActor = FindNearestActor();
	if (!TargetActor)
	{
		return;
		//UE_LOG(LogTemp, Warning, TEXT("!TargetActor"));
	}
		

	if (TargetActor->GetClass()->ImplementsInterface(UInterface_Press::StaticClass()))
	{
		IInterface_Press::Execute_PressE(TargetActor, this);
	}
}

void ARoboPlayer::Server_InteractDoor_Implementation(AActor* Target)
{
	UE_LOG(LogTemp, Warning, TEXT("ARoboPlayer::Server_InteractDoor_Implementation"));
	if (auto* Interactable = Cast<AInteractableActor>(Target))
	{
		UE_LOG(LogTemp, Warning, TEXT("ARoboPlayer::Server_InteractDoor_Implementation_Interactable!!!!"));
		Interactable->InteractDoor(this);
	}
}

void ARoboPlayer::Input_PressF()
{
	UE_LOG(LogTemp, Warning, TEXT("ARoboPlayer::Input_PressF()"));
	if (FocusedActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("ARoboPlayer::Input_PressF()_FocusedActor"));
		Server_InteractDoor(FocusedActor);
	}
}

void ARoboPlayer::Multi_Reload_Implementation()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		PlayAnimMontage(ChildWeapon->ReloadMontage);
	}
}

void ARoboPlayer::Server_Reload_Implementation()
{
	Multi_Reload();
}

void ARoboPlayer::Server_StartFire_Implementation()
{
	DoFire();
}

void ARoboPlayer::Server_StopFire_Implementation()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		ChildWeapon->StopFire();
	}
}

void ARoboPlayer::Multi_PlayerDie_Implementation()
{
	if (bIsPlayerDead)
	{
		return;
	}

	bIsPlayerDead = true;

	if (DeathMontage)
	{
		UAnimInstance* DeathAnim = GetMesh()->GetAnimInstance();
		if (DeathAnim)
		{
			DeathAnim->Montage_Play(DeathMontage);
		}
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
		MoveComp->SetComponentTickEnabled(false);
	}

	if (DeathMontage == nullptr) // 몽타주가 없을 때만 랙돌 실행
	{
		SetActorEnableCollision(false);
		if (GetMesh())
		{
			GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
			GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			GetMesh()->SetSimulatePhysics(true);
		}
	}
	else
	{
		// 몽타주 재생 시에는 애니메이션이 우선이므로 물리 시뮬레이션 끔.
		GetMesh()->SetSimulatePhysics(false);
	}
}

void ARoboPlayer::Multi_PlayerSpawnHitEffect_Implementation(FVector_NetQuantize Location, FRotator Rotation)
{
	if (BloodEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player BloodEffect"));
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BloodEffect,
			Location,
			Rotation
		);
	}
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

void ARoboPlayer::DoFire()
{
	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (ChildWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("ARoboPlayer::DoFire()"));
		PlayAnimMontage(ChildWeapon->FireMontage);
		ChildWeapon->Fire(); 
	}
}

void ARoboPlayer::StartFire()
{
	Server_StartFire();
}

void ARoboPlayer::StopFire()
{
	Server_StopFire();
}

void ARoboPlayer::OnRep_WeaponClass()
{
	SetWeaponChildActor(CurrentWeaponClass);
}

void ARoboPlayer::SetWeaponChildActor(TSubclassOf<AWeaponBase> InWeaponClass)
{

	UE_LOG(LogTemp, Warning, TEXT("ARoboPlayer::SetWeaponChildActor"));
	if (!Weapon || !InWeaponClass)
	{
		return;
	}

	if (CurWeapon)
	{
		CurWeapon->OnBulletChanged.RemoveAll(this);
	}

	Weapon->SetChildActorClass(InWeaponClass);
	Weapon->CreateChildActor();

	AWeaponBase* ChildWeapon = Cast<AWeaponBase>(Weapon->GetChildActor());
	if (!ChildWeapon)
	{
		return;
	}

	CurWeapon = ChildWeapon;

	if (ChildWeapon)
	{
		if (ChildWeapon->Name.Compare(TEXT("Pistol")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Pistol;
			ChildWeapon->SetOwner(this);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(WeaponState));
			ChildWeapon->OnBulletChanged.AddUObject(this, &ARoboPlayer::HandleBulletChanged);
		}
		else if (ChildWeapon->Name.Compare(TEXT("Rifle")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Rifle;
			ChildWeapon->SetOwner(this);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(WeaponState));
			ChildWeapon->OnBulletChanged.AddUObject(this, &ARoboPlayer::HandleBulletChanged);
		}
		else if (ChildWeapon->Name.Compare(TEXT("ScifiKnife")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::ScifiKnife;
			ChildWeapon->SetOwner(this);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(WeaponState));
			ChildWeapon->OnBulletChanged.AddUObject(this, &ARoboPlayer::HandleBulletChanged);
		}
		else if (ChildWeapon->Name.Compare(TEXT("ScifiWeapon01")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Rifle;
			ChildWeapon->SetOwner(this);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(WeaponState));
			ChildWeapon->OnBulletChanged.AddUObject(this, &ARoboPlayer::HandleBulletChanged);
		}
		else if (ChildWeapon->Name.Compare(TEXT("ScifiWeapon02")) == 0)
		{
			ChildWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ChildWeapon->SocketName);
			WeaponState = EWeaponState::Rifle;
			ChildWeapon->SetOwner(this);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *UEnum::GetValueAsString(WeaponState));
			ChildWeapon->OnBulletChanged.AddUObject(this, &ARoboPlayer::HandleBulletChanged);
		}

		HandleBulletChanged(ChildWeapon->CurBullet, ChildWeapon->MaxBullet);
	}
}

void ARoboPlayer::HandleBulletChanged(int32 InCurBullet, int32 InMaxBullet)
{
	if (!IsLocallyControlled())
		return;

	ARoboPlayerController* PC = Cast<ARoboPlayerController>(GetController());

	if (!PC->PlayerWidgetObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("HandleBulletChanged : PlayerWidget is null"));
		return;
	}

	PC->PlayerWidgetObject->UpdateBullet(InCurBullet, InMaxBullet);
}

FRotator ARoboPlayer::GetAimOffset() const
{
	//Player 조준 방향
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}
