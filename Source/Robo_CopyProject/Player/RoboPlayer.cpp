// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboPlayer.h"
#include "RoboPlayerController.h"
#include "RoboPlayerState.h"

#include "GameframeWork/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ChildActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "../Item/PickUpItemBase.h"
#include "../Weapon/WeaponBase.h"

#include "../MapActor/InteractableActor.h"
#include "../MapActor/DoorActor.h"

#include "../Widget/PlayerWidget.h"
#include "../Widget/PlayerStatWidget.h"
#include "../Widget/LevelUpWidget.h"


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

	if (GetMesh())
	{
		GetMesh()->SetIsReplicated(true);
	}
	bReplicates = true;

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
		//GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	}

	//XP Widget 초기값 업데이트
	if (PlayerWidgetObject && PlayerWidgetObject->PlayerStatWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("ARoboPlayer::BeginPlay()_PlayerWidgetObject"));
		PlayerWidgetObject->PlayerStatWidget->ProcessXPBar(CurXP / MaxXP);
	}

	//Delegate
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
	UEnhancedInputComponent* UIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (UIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("UIC"));
		UIC->BindAction(IA_Equip, ETriggerEvent::Started, this, &ARoboPlayer::Input_PressE);
		UIC->BindAction(IA_DoorOpen, ETriggerEvent::Started, this, &ARoboPlayer::Input_PressF);
		UIC->BindAction(IA_Reload, ETriggerEvent::Completed, this, &ARoboPlayer::Server_Reload);

		UIC->BindAction(IA_Fire, ETriggerEvent::Started, this, &ARoboPlayer::StartFire);
		UIC->BindAction(IA_Fire, ETriggerEvent::Completed, this, &ARoboPlayer::StopFire);

		UIC->BindAction(IA_LevelUpBenefit, ETriggerEvent::Completed, this, &ARoboPlayer::Input_LevelUpBenefit); //T
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

void ARoboPlayer::SetLevelUpWidget(ULevelUpWidget* InWidget)
{
	LevelUpWidgetObject = InWidget;
}

void ARoboPlayer::OnRep_CurrentHP()
{
	OnHpChanged.Broadcast(CurHp / MaxHp);
}

void ARoboPlayer::AddPlayerXP(float InAmount)
{
	CurXP += InAmount;

	if (CurXP >= MaxXP)
	{
		CurXP = 0.0f;
		ARoboPlayerState* PS = GetPlayerState<ARoboPlayerState>();
		if (PS)
		{
			PS->LevelUp();
		}

		UE_LOG(LogTemp, Warning, TEXT("LevelUpWidgetObject : %s"), LevelUpWidgetObject ? *LevelUpWidgetObject->GetName() : TEXT("None"));
		if (LevelUpWidgetObject)
		{
			UE_LOG(LogTemp, Warning, TEXT("ARoboPlayer::AddPlayerXP_LevelUpWidgetObject"));
			LevelUpWidgetObject->InitLevelUpScreen(); // 초기 상태 설정
			if (!LevelUpWidgetObject->IsInViewport())
			{
				LevelUpWidgetObject->AddToViewport();
			}
		}
	}

	if (PlayerWidgetObject)
	{
		if (PlayerWidgetObject->PlayerStatWidget)
		{
			PlayerWidgetObject->PlayerStatWidget->ProcessXPBar(CurXP / MaxXP);
		}
	}
}

void ARoboPlayer::ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	ARoboPlayerController* RPC = Cast<ARoboPlayerController>(GetController());
	if (!RPC || !RPC->HasAuthority())
	{
		return;
	}
}

float ARoboPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (CurHp <= 0.0f)
	{
		return DamageAmount;
	}

	CurHp -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("Player TakeDamage : %f"), CurHp);
	OnRep_CurrentHP();
	Multi_PlayerSpawnHitEffect(GetActorLocation(), GetActorRotation());

	if (HasAuthority() && CurHp <= 0.0f)
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
	if (auto* Interactable = Cast<AInteractableActor>(Target))
	{
		Interactable->InteractDoor(this);
	}
}

void ARoboPlayer::Input_PressF()
{
	if (FocusedActor)
	{
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

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
		//MoveComp->SetComponentTickEnabled(false);
	}
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (DeathMontage)
	{
		UAnimInstance* DeathAnim = GetMesh()->GetAnimInstance();
		if (DeathAnim)
		{
			float Duration = DeathAnim->Montage_Play(DeathMontage);
			UE_LOG(LogTemp, Log, TEXT("Montage Play Duration: %f"), Duration);
		}
	}
}

void ARoboPlayer::Multi_PlayerSpawnHitEffect_Implementation(FVector_NetQuantize Location, FRotator Rotation)
{
	if (TakeDamageEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			TakeDamageEffect,
			Location,
			Rotation
		);
	}
}

void ARoboPlayer::Input_LevelUpBenefit()
{
	if (LevelUpWidgetObject && LevelUpWidgetObject->IsInViewport())
	{
		LevelUpWidgetObject->ToggleScreen();
		UE_LOG(LogTemp, Log, TEXT("T Key Pressed: Toggling LevelUp Screens"));
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

void ARoboPlayer::OnDeathMontageEnded()
{
	if (GetMesh())
	{
		// 날아가는 것 방지하도록 물리엔진 초기화
		GetMesh()->SetAllBodiesBelowSimulatePhysics(NAME_None, false);
		GetMesh()->SetPhysicsLinearVelocity(FVector::ZeroVector);
		GetMesh()->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);


		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationCustomMode);
	}
}
