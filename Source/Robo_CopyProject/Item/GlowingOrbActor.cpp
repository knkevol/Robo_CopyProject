// Fill out your copyright notice in the Description page of Project Settings.


#include "GlowingOrbActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

// Sets default values
AGlowingOrbActor::AGlowingOrbActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	bReplicates = true;
	SetReplicateMovement(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Root);

	OrbCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OrbCollision"));
	OrbCollision->SetupAttachment(Root);
	OrbCollision->SetSphereRadius(60.0f);
	OrbCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	OrbCollision->SetGenerateOverlapEvents(true);

	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	OrbMesh->SetupAttachment(Root);
	OrbMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OrbMesh->SetIsReplicated(true);

	OrbFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("OrbFX"));
	OrbFX->SetupAttachment(Root);
	OrbFX->SetAutoActivate(false);

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
	OrbCollision->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);

	if (OrbFX)
	{
		if (bVisible)
		{
			OrbFX->Activate();
		}
		else
		{
			OrbFX->Deactivate();
		}
	}
}

void AGlowingOrbActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!bOrbExist)
	{
		return;
	}

	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (!Player)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Heal Orb consumed: +%f"), HealAmount);

	bOrbExist = false;
	OnRep_OrbExist();

	Destroy();

	
}

void AGlowingOrbActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGlowingOrbActor, bOrbExist);
	DOREPLIFETIME(AGlowingOrbActor, HealAmount);
}

