// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoboBossMonster.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossChangedHP, const float, Percent);

UENUM(BlueprintType)
enum class EBMonsterState : uint8
{
	Normal = 0 UMETA(DisplayName = "Normal"),
	Battle = 2 UMETA(DisplayName = "Battle"),
	Death = 3 UMETA(DisplayName = "Death")
};

UCLASS()
class ROBO_COPYPROJECT_API ARoboBossMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoboBossMonster();

	float GetCurrentHP() {return CurrentHP; }
	float GetMaxHP() {return MaxHP; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//----------RPC-------------
	UFUNCTION(NetMulticast, Reliable)
	void Multi_BossMonsterDie();
	void Multi_BossMonsterDie_Implementation();

	//---------------------------


	//-----------------------HP--------------------
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat", ReplicatedUsing = "OnRep_BMonsterCurrentHP")
	float CurrentHP = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat", Replicated)
	float MaxHP = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat", Replicated)
	EBMonsterState CurrentState;

	UFUNCTION()
	void OnRep_BMonsterCurrentHP();
	//------------------------------------------------

	//---------------Perception-----------------------
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	//------------------------------------------------

	//------------------Attack---------------
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//---------------------------------------
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE const EBMonsterState GetCurrentState() { return CurrentState; }

	void SetState(EBMonsterState NewState);

	//-------------------Boss Widget(HP/Name)-------------------
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat")
	FName BossName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat")
	TObjectPtr<class UPlayerTopWidget> TopWidget;

	void SetPlayerTopWidget(UPlayerTopWidget* InWidget) { TopWidget = InWidget; }

	UPROPERTY(BlueprintAssignable)
	FOnBossChangedHP OnBossHpChanged;
	//-----------------------------------------------

	//--------------------------AI--------------------
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat")
	TObjectPtr<class UBehaviorTree> BTAsset;
	//------------------------------------------------

	//----------------Attack----------------
	// 공격 반지름 (구체 크기)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat")
	float AttackRadius = 700.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat")
	float AttackRange = 30.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat")
	float AttackDamage = 10.0f;

	void ProcessAttackHit_Boss();
	//------------------------------------
};
