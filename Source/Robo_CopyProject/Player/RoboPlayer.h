// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "../Interface/Interface_Press.h"
#include "RoboPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedHP, const float, Percent);

class UInputAction;
class AWeaponBase;
class UAIPerceptionStimuliSourceComponent;

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	Unarmed = 0 UMETA(DisplayName = "Unarmed"),
	Pistol = 1 UMETA(DisplayName = "Pistol"),
	Rifle = 10 UMETA(DisplayName = "Rifle"),
	ScifiKnife = 20 UMETA(DisplayName = "ScifiKnife"),
	ScifiWeapon01 = 30 UMETA(DisplayName = "ScifiWeapon01"),
	ScifiWeapon02 = 40 UMETA(DisplayName = "ScifiWeapon02")
};

UCLASS()
class ROBO_COPYPROJECT_API ARoboPlayer : public ACharacter, public IInterface_Press, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoboPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// -------Bind (Network)
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// -----Player Data-----
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter", Replicated)
	EWeaponState WeaponState = EWeaponState::Unarmed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UChildActorComponent> Weapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter")
	TObjectPtr<class AWeaponBase> CurWeapon = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter")
	uint8 bIsPlayerDead : 1 = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter", ReplicatedUsing = "OnRep_CurrentHP")
	float CurHp = 200;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter", Replicated)
	float MaxHp = 200;

	UFUNCTION()
	void OnRep_CurrentHP();

	// 복제 통지 함수
	UPROPERTY(BlueprintAssignable)
	FOnChangedHP OnHpChanged;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter")
	float CurXP = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter")
	float MaxXP = 100.0f;

	void AddPlayerXP(float InAmount);

	//-----------------------Widget----------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoboWidget")
	TObjectPtr<class UPlayerWidget> PlayerWidgetObject;

	void SetPlayerWidget(class UPlayerWidget* InWidget);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoboWidget")
	TObjectPtr<class ULevelUpWidget> LevelUpWidgetObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RoboWidget")
	TSubclassOf<class ULevelUpWidget> LevelUpWidgetClass;
	//------------------------------------------------------------------
	
	// -------------Delegate
	UFUNCTION()
	void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);



	// -------Find Actor
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* FindNearestActor() const;

	void EquipItem(class APickUpItemBase* PickedItem);
	void UseItem(class APickUpItemBase* PickedItem);

	// --------RPC
	UFUNCTION(NetMulticast, Reliable)
	void Multi_EquipItem(class APickUpItemBase* PickedItem);
	void Multi_EquipItem_Implementation(class APickUpItemBase* PickedItem);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_EquipWeapon(TSubclassOf<AWeaponBase> WeaponClass);
	void Server_EquipWeapon_Implementation(TSubclassOf<AWeaponBase> WeaponClass);


	UFUNCTION(BlueprintCallable)
	void Input_PressE();

	UFUNCTION(Server, Reliable)
	void Server_InteractDoor(AActor* Target);
	void Server_InteractDoor_Implementation(AActor* Target);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter")
	TObjectPtr<class AActor> FocusedActor;

	UFUNCTION(BlueprintCallable)
	void Input_PressF();

	UFUNCTION(NetMulticast, Reliable)
	void Multi_Reload();
	void Multi_Reload_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_Reload();
	void Server_Reload_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_StartFire();
	void Server_StartFire_Implementation();

	UFUNCTION(Server, Reliable)
	void Server_StopFire();
	void Server_StopFire_Implementation();


	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayerDie();
	void Multi_PlayerDie_Implementation();

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayerSpawnHitEffect(FVector_NetQuantize Location, FRotator Rotation);
	void Multi_PlayerSpawnHitEffect_Implementation(FVector_NetQuantize Location, FRotator Rotation);


	//----------------Input Action
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboInput")
	TObjectPtr<UInputAction> IA_Equip;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboInput")
	TObjectPtr<UInputAction> IA_Reload;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboInput")
	TObjectPtr<UInputAction> IA_Fire;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboInput")
	TObjectPtr<UInputAction> IA_DoorOpen;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboInput")
	TObjectPtr<UInputAction> IA_LevelUpBenefit;

	//Not Network
	void Input_LevelUpBenefit();


	// ---------------Montage
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboMontage")
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboMontage")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboEffect")
	TObjectPtr<UParticleSystem> TakeDamageEffect;


	// ---------------AI
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter")
	TObjectPtr<UAIPerceptionStimuliSourceComponent> StimuliSource;


	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void DoFire();

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter", ReplicatedUsing = "OnRep_WeaponClass")
	TSubclassOf<AWeaponBase> CurrentWeaponClass;

	UFUNCTION()
	void OnRep_WeaponClass();


	UFUNCTION(BlueprintCallable)
	void SetWeaponChildActor(TSubclassOf<AWeaponBase> InWeaponClass);

	void HandleBulletChanged(int32 InCurBullet, int32 InMaxBullet);


	// ----------Player Aim
	FRotator GetAimOffset() const;

	// ----------------Player Death
	void OnDeathMontageEnded();

};
