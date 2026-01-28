// Fill out your copyright notice in the Description page of Project Settings.


#include "GlowingOrbActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "../Player/RoboPlayer.h"

// Sets default values
AGlowingOrbActor::AGlowingOrbActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	bReplicates = true;
	SetReplicateMovement(true);

	OrbCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OrbCollision"));
	SetRootComponent(OrbCollision);
	OrbCollision->SetSphereRadius(10.0f);
	OrbCollision->SetCollisionProfileName(TEXT("PhysicsActor"));
	OrbCollision->SetGenerateOverlapEvents(true);
	OrbCollision->SetSimulatePhysics(true);
	OrbCollision->SetEnableGravity(true);

	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	OrbMesh->SetupAttachment(OrbCollision);
	OrbMesh->SetIsReplicated(true);
	OrbMesh->SetSimulatePhysics(false);
	OrbMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OrbMesh->SetNotifyRigidBodyCollision(true); // 충돌 시 이벤트 발생 필요하면 켬
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void AGlowingOrbActor::BeginPlay()
{
	Super::BeginPlay();

	OrbCollision->OnComponentBeginOverlap.AddDynamic(this, &AGlowingOrbActor::OnOverlapBegin);
	//클라이언트 접속 타이밍 대응
	OnRep_OrbExist();
	
}

// Called every frame
void AGlowingOrbActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGlowingOrbActor::OnRep_OrbExist()
{
	const bool bVisible = bOrbExist;

	OrbMesh->SetVisibility(bVisible);

	if (bVisible)
	{
		// 핵심: 물리를 사용할 때는 QueryAndPhysics를 써야 오버랩과 물리 튕김이 둘 다 작동합니다.
		OrbCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	else
	{
		OrbCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AGlowingOrbActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority() || !bOrbExist)
	{
		UE_LOG(LogTemp, Log, TEXT("AGlowingOrbActor::OnOverlapBegin_!HasAuthority() || !bOrbExist"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("AGlowingOrbActor::OnOverlapBegin"));

	ARoboPlayer* Player = Cast<ARoboPlayer>(OtherActor);
	if (Player)
	{
		Player->CurHp = FMath::Clamp(Player->CurHp + HealAmount, 0.0f, Player->MaxHp);
		Player->OnRep_CurrentHP(); //UI

		bOrbExist = false;
		OnRep_OrbExist();

		SetLifeSpan(1.0f);
	}
}

void AGlowingOrbActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGlowingOrbActor, bOrbExist);
	DOREPLIFETIME(AGlowingOrbActor, HealAmount);
}

