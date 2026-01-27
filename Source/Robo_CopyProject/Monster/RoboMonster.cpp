// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboMonster.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "RoboMonster_AIC.h"
#include "Net/UnrealNetwork.h" //Replicated
#include "Components/ProgressBar.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ARoboMonster::ARoboMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MonsterHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterHPWidget"));
	MonsterHPWidget->SetupAttachment(GetMesh());
	MonsterHPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	MonsterHPWidget->SetWidgetSpace(EWidgetSpace::Screen);
	MonsterHPWidget->SetIsReplicated(true);

}

// Called when the game starts or when spawned
void ARoboMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoboMonster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoboMonster, CurrentHP);
	DOREPLIFETIME(ARoboMonster, MaxHP);
}

void ARoboMonster::OnRep_MonsterCurrentHP()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_MonsterCurrentHP"));
	UpdateMonsterHPBar();
}

void ARoboMonster::UpdateMonsterHPBar()
{
	if (MonsterHPWidget)
	{
		UUserWidget* HPWidget = Cast<UUserWidget>(MonsterHPWidget->GetUserWidgetObject());
		if (HPWidget)
		{
			// 위젯 내의 ProgressBar 찾기 (이름이 "HPBar"인 경우)
			UProgressBar* HPBar = Cast<UProgressBar>(HPWidget->GetWidgetFromName(TEXT("MonsterHpBar")));
			if (HPBar)
			{
				float Percent = CurrentHP / MaxHP;
				HPBar->SetPercent(Percent);
				UE_LOG(LogTemp, Warning, TEXT("HP Update: %f / %f = %f"), CurrentHP, MaxHP, Percent);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("HPWidget X"));
		}
	}
}

void ARoboMonster::Multi_MonsterDie_Implementation()
{
	if (HasAuthority())
	{
		if (CurrentHP <= 0)
		{
			SetState(EMonsterState::Death);
			ARoboMonster_AIC* AIC = Cast<ARoboMonster_AIC>(GetController());
			if (AIC)
			{
				AIC->SetState(EMonsterState::Death);
			}
		}
	}

	GetController()->SetActorEnableCollision(false);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ARoboMonster::Multi_SpawnHitEffect_Implementation(FVector_NetQuantize Location, FRotator Rotation)
{
	if (BloodEffect)
	{
		UE_LOG(LogTemp, Warning, TEXT("BloodEffect"));
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			BloodEffect,
			Location,
			Rotation
		);
	}
}

// Called every frame
void ARoboMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoboMonster::SetState(EMonsterState NewState)
{
	CurrentState = NewState;
}

float ARoboMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);


	if (CurrentHP <= 0.0f)
	{
		return DamageAmount;
	}

	CurrentHP -= DamageAmount;
	UpdateMonsterHPBar();
	Multi_SpawnHitEffect(GetActorLocation(), GetActorRotation());

	if (CurrentHP <= 0.0f)
	{
		Multi_MonsterDie();
	}

	return DamageAmount;
}

void ARoboMonster::ChangeSpeed(float NewMaxSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewMaxSpeed;
}

