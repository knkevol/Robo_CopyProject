// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat")
	EMonsterState CurrentState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat")
	float CurrentHP = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MonsterStat")
	float MaxHP = 100;

	void SetState(EMonsterState NewState);

	FORCEINLINE const EMonsterState GetCurrentState() { return CurrentState; }

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};
