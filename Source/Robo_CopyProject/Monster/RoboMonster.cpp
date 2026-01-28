// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboMonster.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
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
	DOREPLIFETIME(ARoboMonster, CurrentState);
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
	if (!IsValid(this))
	{
		return;
	}

	SetState(EMonsterState::Death);

	if (HasAuthority())
	{
		if (CurrentHP <= 0)
		{
			ARoboMonster_AIC* AIC = Cast<ARoboMonster_AIC>(GetController());
			if (AIC)
			{
				AIC->SetState(EMonsterState::Death);
			}
		}  
	}

	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
		MoveComp->SetComponentTickEnabled(false); // 더 이상 위치 보정을 하지 않음
	}

	SetActorEnableCollision(false);
	if (GetMesh())
	{
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->SetSimulatePhysics(true);
	}
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
	if (HasAuthority())
	{
		CurrentState = NewState;
	}
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

void ARoboMonster::ProcessAttackHit()
{
	if (!HasAuthority())
	{
		return;
	}

	FHitResult HitResult;
	// 공격 시작 지점: 몬스터 위치에서 약간 앞쪽
	FVector Start = GetActorLocation() + GetActorForwardVector() * 50.0f;
	// 공격 종료 지점: 시작 지점에서 사거리만큼 앞쪽
	FVector End = Start + GetActorForwardVector() * AttackRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHasHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

	// 디버그 드로잉
	FColor DrawColor = bHasHit ? FColor::Green : FColor::Red;
	DrawDebugSphere(GetWorld(), End, AttackRadius, 16, DrawColor, false, 2.0f);

	// 데미지 전달
	if (bHasHit)
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			// Player->TakeDamage
			UGameplayStatics::ApplyDamage(
				HitActor,
				AttackDamage,
				GetController(),
				this,
				UDamageType::StaticClass()
			);
			UE_LOG(LogTemp, Log, TEXT("Monster Hit Actor: %s"), *HitActor->GetName());
		}
	}
}

