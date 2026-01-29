// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "RoboMonster.generated.h"

UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Normal = 0 UMETA(DisplayName = "Normal"),
	Chase = 1 UMETA(DisplayName = "Chase"),
	Battle = 2 UMETA(DisplayName = "Battle"),
	Death = 3 UMETA(DisplayName = "Death")
};

UCLASS()
class ROBO_COPYPROJECT_API ARoboMonster : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARoboMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ---------------HP -----------------
	UFUNCTION()
	void OnRep_MonsterCurrentHP();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat")
	TObjectPtr<UWidgetComponent> MonsterHPWidget;

	void UpdateMonsterHPBar(); // 실제 UI를 갱신하는 내부 함수
	//---------------------------------------------------

	// ---------------RPC-----------------
	UFUNCTION(NetMulticast, Reliable)
	void Multi_MonsterDie();
	void Multi_MonsterDie_Implementation();

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_SpawnHitEffect(FVector_NetQuantize Location, FRotator Rotation);
	void Multi_SpawnHitEffect_Implementation(FVector_NetQuantize Location, FRotator Rotation);
	//---------------------------------------------------
	
	//-------------Drop Item---------
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterDropItems")
	TSubclassOf<class AGlowingOrbActor> OrbClass;

	void SpawnGlowingOrb();
	//------------------------------------


	//-------------Death---------
	void AfterDie(AActor* InTargetPlayer);
	//------------------------------------

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//---------------MonsterStat-----------------
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat", ReplicatedUsing = "OnRep_MonsterCurrentHP")
	float CurrentHP = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat", Replicated)
	float MaxHP = 100;

	void SetState(EMonsterState NewState);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat", Replicated)
	EMonsterState CurrentState;

	FORCEINLINE const EMonsterState GetCurrentState() { return CurrentState; }

	UFUNCTION(BlueprintCallable)
	void ChangeSpeed(float NewMaxSpeed);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat")
	float XPValue = 100.0f; //temp
	//---------------------------------------------------


	//------------------Attack-----------------
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MonsterEffect")
	TObjectPtr<UParticleSystem> TakeDamageEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat")
	float AttackRange = 100.0f;

	// 공격 반지름 (구체 크기)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat")
	float AttackRadius = 50.0f;

	// 공격 데미지
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat")
	float AttackDamage = 10.0f;

	void ProcessAttackHit();
	//-------------------------------------------------------

	

};
