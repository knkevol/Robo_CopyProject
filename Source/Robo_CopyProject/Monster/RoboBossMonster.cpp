// Fill out your copyright notice in the Description page of Project Settings.


#include "RoboBossMonster.h"
#include "Net/UnrealNetwork.h" //Replicated
#include "../Player/RoboPlayer.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"

#include "../Widget/PlayerTopWidget.h"
#include "Components/ProgressBar.h"


// Sets default values
ARoboBossMonster::ARoboBossMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MonsterCapsule"));
	GetMesh()->SetCollisionProfileName(TEXT("MonsterMesh"));


	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ARoboBossMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARoboBossMonster::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARoboBossMonster, CurrentHP);
	DOREPLIFETIME(ARoboBossMonster, MaxHP);
	DOREPLIFETIME(ARoboBossMonster, CurrentState);
}

void ARoboBossMonster::SetState(EBMonsterState NewState)
{
	if (HasAuthority())
	{
		CurrentState = NewState;
	}
}

void ARoboBossMonster::ProcessAttackHit_Boss()
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
	UE_LOG(LogTemp, Warning, TEXT("ARoboBossMonster::ProcessAttackHit_Boss()"));
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
		UE_LOG(LogTemp, Warning, TEXT("ARoboBossMonster::ProcessAttackHit_Boss()_bHasHit"));
		for (auto& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();

			if (HitActor && HitActor->IsA(ARoboPlayer::StaticClass()))
			{
				UE_LOG(LogTemp, Warning, TEXT("ARoboBossMonster::ProcessAttackHit_Boss()_HitActor"));
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
	//DrawDebugSphere(GetWorld(), AttackCenter, AttackRadius, 16, bHasHit ? FColor::Green : FColor::Red, false, 1.0f);
}

void ARoboBossMonster::OnRep_BMonsterCurrentHP()
{
	OnBossHpChanged.Broadcast(CurrentHP / MaxHP);
}

void ARoboBossMonster::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);

	OutLocation.Z -= 110.f;
}

// Called every frame
void ARoboBossMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARoboBossMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

