// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboMonster.h"
#include "RoboMonster_AIC.h"
#include "../Item/GlowingOrbActor.h"
#include "../Player/RoboPlayer.h"

#include "Engine/DamageEvents.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"

#include "Components/ProgressBar.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Net/UnrealNetwork.h" //Replicated

#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"

// Sets default values
ARoboMonster::ARoboMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCapsule"));
	GetMesh()->SetCollisionProfileName(TEXT("MonsterMesh"));

	MonsterHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MonsterHPWidget"));
	MonsterHPWidget->SetupAttachment(GetMesh());
	MonsterHPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
	MonsterHPWidget->SetWidgetSpace(EWidgetSpace::Screen);
	MonsterHPWidget->SetIsReplicated(true);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

// Called when the game starts or when spawned
void ARoboMonster::BeginPlay()
{
	Super::BeginPlay();

	//handle Spawn Monster
	if (MonsterHPWidget)
	{
		MonsterHPWidget->RegisterComponent();
	}
	
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

void ARoboMonster::UpdateHPBarVisibility()
{
	UE_LOG(LogTemp, Warning, TEXT("ARoboMonster::UpdateHPBarVisibility()"));
	if (!MonsterHPWidget)
	{
		// 최적화: 위젯이 아예 꺼져있거나 컴포넌트가 없으면 계산 안 함
		return;
	}

	// 1. 카메라 위치 가져오기
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	if (!CameraManager) return;

	FVector CameraLocation = CameraManager->GetCameraLocation();
	FVector WidgetLocation = MonsterHPWidget->GetComponentLocation();

	const float MaxVisibleDistance = 2000.0f;
	float DistanceSquared = FVector::DistSquared(CameraLocation, WidgetLocation);

	if (DistanceSquared > FMath::Square(MaxVisibleDistance))
	{
		// 너무 멀면 아예 안 보이게 처리하고 종료
		if (MonsterHPWidget->IsVisible())
		{
			MonsterHPWidget->SetVisibility(false);
		}
		return;
	}

	// 3. Trace 설정
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 나 자신(몬스터)은 무시

	// 플레이어 캐릭터도 무시하고 싶다면 추가
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn) Params.AddIgnoredActor(PlayerPawn);

	// 4. Line Trace 발사 (Visibility 채널 사용)
	// 장애물(Wall, StaticMesh 등)이 중간에 있으면 true 리턴
	bool bIsBlocked = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		CameraLocation,
		WidgetLocation,
		ECC_Visibility,
		Params
	);

	// 5. 위젯 컴포넌트의 가시성 조절
	// 장애물에 가려졌다면 위젯을 숨김 (Hidden), 아니면 보여줌 (Visible)
	// 단, 위젯 자체의 투명도를 조절하는 것이 아니라 컴포넌트 렌더링을 끄는 방식입니다.
	if (bIsBlocked)
	{
		// 무언가에 가려짐 -> 위젯 숨기기
		MonsterHPWidget->SetVisibility(false);
	}
	else
	{
		// 가려지지 않음 -> 위젯 보여주기
		MonsterHPWidget->SetVisibility(true);
	}

	// [디버그용] 실제 선이 어떻게 그려지는지 보고 싶다면 주석 해제
	//DrawDebugLine(GetWorld(), CameraLocation, WidgetLocation, bIsBlocked ? FColor::Red : FColor::Green, false, -1.0f, 0, 2.0f);
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
		SpawnGlowingOrb();
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

// Called every frame
void ARoboMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	ElapsedTime += DeltaTime;

	if (ElapsedTime >= 0.1f)
	{
		UpdateHPBarVisibility();
		ElapsedTime = 0.0f;
	}
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
		if (EventInstigator)
		{
			AfterDie(EventInstigator->GetPawn());
		}
		else
		{
			//예외처리
		}
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
	FVector AttackCenter = GetActorLocation() + GetActorForwardVector() * AttackRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape AttackSphere = FCollisionShape::MakeSphere(AttackRadius);

	bool bHasHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		AttackCenter,
		FQuat::Identity,
		ECC_Pawn, // 또는 플레이어 전용 채널
		AttackSphere,
		Params
	);

	if (bHasHit)
	{
		for (auto& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();

			if (HitActor && HitActor->IsA(ARoboPlayer::StaticClass()))
			{
				UGameplayStatics::ApplyDamage(
					HitActor,
					AttackDamage,
					GetController(),
					this,
					UDamageType::StaticClass()
				);
				break;  //한번에 한명만 피격
			}
		}
	}
	DrawDebugSphere(GetWorld(), AttackCenter, AttackRadius, 16, bHasHit ? FColor::Green : FColor::Red, false, 1.0f);
}

void ARoboMonster::SpawnGlowingOrb()
{
	if (OrbClass == nullptr)
	{
		return;
	}

	float OffsetX = FMath::RandRange(50.f, 100.f);
	float OffsetY = FMath::RandRange(50.f, 100.f);

	FVector MonsterLocation = GetActorLocation();
	int32 SpawnCount = 3;

	for (int32 i = 0; i < SpawnCount; i++)
	{
		float RandomAngle = FMath::RandRange(0.f, 360.f);
		float AngleRad = FMath::DegreesToRadians(RandomAngle);
		float RandomDist = FMath::RandRange(150.f, 200.f);

		FVector FinalSpawnLocation = MonsterLocation;
		FinalSpawnLocation.X += FMath::Cos(AngleRad) * RandomDist;
		FinalSpawnLocation.Y += FMath::Sin(AngleRad) * RandomDist;
		FinalSpawnLocation.Z -= 50.f; // 바닥 높이 조정

		FActorSpawnParameters Params;
		Params.Owner = this;

		GetWorld()->SpawnActor<AGlowingOrbActor>(OrbClass, FinalSpawnLocation, FRotator::ZeroRotator, Params);
	}
	
}

void ARoboMonster::AfterDie(AActor* InTargetPlayer)
{
	ARoboPlayer* Player = Cast<ARoboPlayer>(InTargetPlayer);
	if (Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Monster XP : %f"), XPValue);
		Player->AddPlayerXP(XPValue);
	}
}

