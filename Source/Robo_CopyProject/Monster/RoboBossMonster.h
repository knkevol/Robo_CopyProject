// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoboBossMonster.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


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
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE const EBMonsterState GetCurrentState() { return CurrentState; }

	void SetState(EBMonsterState NewState);

	//--------------------------AI--------------------
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat")
	TObjectPtr<class UBehaviorTree> BTAsset;
	//------------------------------------------------

	//----------------Attack----------------
	// 공격 반지름 (구체 크기)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat")
	float AttackRadius = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat")
	float AttackRange = 300.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BMonsterStat")
	float AttackDamage = 10.0f;

	void ProcessAttackHit_Boss();
	//------------------------------------
};
