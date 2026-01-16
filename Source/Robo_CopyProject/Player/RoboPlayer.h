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
class ROBO_COPYPROJECT_API ARoboPlayer : public ACharacter, public IInterface_Press
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoboPlayer();

	virtual void PressG_Implementation(ACharacter* Character) override;
	virtual void ReleaseG_Implementation() override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerWidget")
	TObjectPtr<class UPlayerWidget> PlayerWidgetObject;

	void SetPlayerWidget(class UPlayerWidget* InWidget);

	UFUNCTION()
	void OnRep_CurrentHP();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboPlayerStat", ReplicatedUsing = "OnRep_CurrentHP")
	float CurHp = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboPlayerStat", Replicated)
	float MaxHp = 100;

	// 복제 통지 함수
	UPROPERTY(BlueprintAssignable)
	FOnChangedHP OnHpChanged;
	
	// -------------Delegate
	UFUNCTION()
	void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	// -------Find Actor
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* FindNearestActor() const;

	void EquipItem(class APickUpItemBase* PickedItem);
	void UseItem(class APickUpItemBase* PickedItem);

	// --------RPC
	UFUNCTION(NetMulticast, Reliable)
	void Multi_EquipItem(class APickUpItemBase* PickedItem);
	void Multi_EquipItem_Implementation(class APickUpItemBase* PickedItem);

	UFUNCTION(Server, Reliable)
	void Server_PressG();
	void Server_PressG_Implementation();

	UFUNCTION(BlueprintCallable)
	void Input_PressG();

	void PressNearestItem();

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

	//----------------Input Action
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboInput")
	TObjectPtr<UInputAction> IA_Equip;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboInput")
	TObjectPtr<UInputAction> IA_Reload;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboInput")
	TObjectPtr<UInputAction> IA_Fire;


	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	void DoFire();

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboData")
	TSubclassOf<AWeaponBase> WeaponClass;

	UFUNCTION(BlueprintCallable)
	void SetWeaponChildActor(TSubclassOf<AWeaponBase> InWeaponClass);

	void HandleBulletChanged(int32 InCurBullet, int32 InMaxBullet);




	// ----------Player Aim
	FRotator GetAimOffset() const;

};
