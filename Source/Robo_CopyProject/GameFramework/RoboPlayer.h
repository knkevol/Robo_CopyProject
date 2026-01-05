// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "RoboPlayer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangedHP, const float, Percent);

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
class ROBO_COPYPROJECT_API ARoboPlayer : public ACharacter
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
	
	// -----Player State-----
	UFUNCTION()
	void OnRep_CurrentHP();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboPlayerStat", ReplicatedUsing = "OnRep_CurrentHP")
	float CurHp = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboPlayerStat", Replicated)
	float MaxHp = 100;

	// 복제 통지 함수
	UPROPERTY(BlueprintAssignable)
	FOnChangedHP OnHpChanged;
	// Bind (Network)-------

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter", Replicated)
	EWeaponState WeaponState = EWeaponState::Unarmed;

	// Delegate
	UFUNCTION()
	void ProcessBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);


	// -----Weapon
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RoboCharacter", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UChildActorComponent> Weapon;

	void EquipItem(class APickUpItemBase* PickedItem);
	void UseItem(class APickUpItemBase* PickedItem);


};
